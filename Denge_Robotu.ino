//Kütüphaneler eklenir
#include <PID_v1.h>
#include <LMotorController.h>
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

//tanmalamar yapılır
#define LOG_INPUT 0
#define MANUAL_TUNING 0
#define LOG_PID_CONSTANTS 0 
#define MOVE_BACK_FORTH 0

#define MIN_ABS_SPEED 30 // Minumum Başlangıç Hızı

//MPU6050 sensörü isimlendirilir
MPU6050 mpu;

// MPU sensörünün kontrol ve durum varyasyonları belirlenir
bool dmpReady = false;  //DMP init başarılı olmuşsa doğru olarak ayarlayın.
uint8_t mpuIntStatus;   // MPU'dan gerçek kesme durumu baytı tutar
uint8_t devStatus;      // Her cihaz çalışmasından sonra dönüş durumu (0 = başarı,! 0 = hata) (0 = success, !0 = error)
uint16_t packetSize;    // Beklenen DMP paket boyutu (varsayılan 42 bayt)
uint16_t fifoCount;     // Şu anda FIFO'daki tüm baytların sayısı
uint8_t fifoBuffer[128]; // FIFO depolama arabelleği

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         Kuarterna konteyner
VectorFloat gravity;    // [x, y, z]            Yerçekimi vektörü
float ypr[3];           // [yaw, pitch, roll]  Yaw / pitch Adım/ roll rulo container ve yerçekimi vektörü

//PID kontrolcüsünün durumları belirlenir
#if MANUAL_TUNING
  double kp , ki, kd;
  double prevKp, prevKi, prevKd;
#endif
double originalSetpoint = 174.29;
double setpoint = originalSetpoint;
double movingAngleOffset = 0.3;
double input, output;
int moveState=0; //0 = denge ; 1 = geri ; 2 = ileri

#if MANUAL_TUNING
  PID pid(&input, &output, &setpoint, 0, 0, 0, DIRECT);
#else
  PID pid(&input, &output, &setpoint, 70, 240, 1.9, DIRECT);
#endif

//MOTOR SURUCU

int ENA = 3;
int IN1 = 4;
int IN2 = 8;
int IN3 = 5;
int IN4 = 7;
int ENB = 6;


LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, 0.6, 1);

//fonksiyon tanımlamalrı yapılır
//Zamanlama
long time1Hz = 0;
long time5Hz = 0;


// MPU kesme pininin yükselip yükselmediğini gösterir
volatile bool mpuInterrupt = false;    

void dmpDataReady()
{
    mpuInterrupt = true;
}

void setup()
{
    // I2C protokolü başlatılır(I2Cdev kitaplığı bunu otomatik olarak yapmaz)

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // seri haberleşme başlatılır.
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately  Leonardo numaralandırmasını bekle, diğerleri hemen devam et


    // MPU sensörü başlatılır
    Serial.println(F("I2C Kuruluyor..."));
    mpu.initialize();

    //Bağalntı kontrolü yapılır

    Serial.println(F("Suruculer test ediliyor..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 baglanti basarili") : F("MPU6050 baglanti basarisiz"));

    //Sönsürun DMP kurulumu yapılır
    Serial.println(F("DMP kuruluyor..."));
    devStatus = mpu.dmpInitialize();

    // Minimum hassasiyet için ölçeklendirilmiş kendi gyro ofsetlerinizi buradan sağlayın.

    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1688); //Test çipi için 1688 fabrika varsayılanı


    // Eğer çalışıyorsa 0 döndürülür
    if (devStatus == 0)
    {
        Serial.println(F("DMP etkinleştirilir..."));
        mpu.setDMPEnabled(true);
        
        //Arduino kesinti algılaması etkinleştirilir
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        //DMP hazır (dmpReady) değişkeni, ana döngü() işlevinin onu kullanmanın uygun olduğunu bilmesi için true olarak ayarlanır.
        Serial.println(F("DMP hazır! İlk kesintiyi bekleniyor..."));
        dmpReady = true;

        //Daha sonra karşılaştırma için beklenen DMP paket boyutu alınır.

        packetSize = mpu.dmpGetFIFOPacketSize();
        
        //PID kurulumu yapılıyor
        
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-255, 255);  
    }
    else
    {
        // HATA!
        // 1 = başlangıç ​​bellek yükü başarısız
        // 2 = DMP yapılandırma güncellemeleri başarısız oldu
        // (eğer kırılırsa, genellikle kod 1 olur)
        Serial.print(F("DMP kurulumu hatası (kod "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}
void loop()
{
    //Programlama başarısız olursa, MPU kesintisini veya ek paketlerin mevcut olmasını beklemek için hiçbir şey yapmayın
    while (!mpuInterrupt && fifoCount < packetSize)
    {
        //mpu verisi yoksa - PID hesaplamaları yapılır ve motorlardan çıktı alınır
        
        pid.Compute();
        motorController.move(output, MIN_ABS_SPEED);
        
        unsigned long currentMillis = millis();

        if (currentMillis - time1Hz >= 1000)
        {
            loopAt1Hz();
            time1Hz = currentMillis;
        }
        
        if (currentMillis - time5Hz >= 5000)
        {
            loopAt5Hz();
            time5Hz = currentMillis;
        }
    }

    // kesme bayrağını sıfırlanır ve INT_STATUS baytı alınır
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // Güncel FIFO sayacı alınır
    fifoCount = mpu.getFIFOCount();

    // taşma olup olmadığı kontrol edilir (kodumuz çok verimsiz olmadıkça bu asla olmamalıdır)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // temiz bir şekilde devam edebilmek için sıfırlanır
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // aksi takdirde, DMP verisi hazır kesmesi kontrol edilir (bu sık sık yapılmalıdır)
    }
    else if (mpuIntStatus & 0x02)
    {
        //doğru kullanılabilir veri uzunluğunu bekleyin, ÇOK kısa bir bekleme olmalıdır
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // FIFO'dan paket okunur
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        //paket sayısının 1'den büyük olması durumunda FIFO sayısı buradan takip edilir.
        //(bu, bir kesinti beklemeden hemen daha fazlasını okumamızı sağlamaktadır)
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        #if LOG_INPUT
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
        #endif
        input = ypr[1] * 180/M_PI + 180;
   }
}


void loopAt1Hz()
{
#if MANUAL_TUNING
    setPIDTuningValues();
#endif
}


void loopAt5Hz()
{
    #if MOVE_BACK_FORTH
        moveBackForth();
    #endif
}


//ileri geri hareket etme


void moveBackForth()
{
    moveState++;
    if (moveState > 2) moveState = 0;
    
    if (moveState == 0)
      setpoint = originalSetpoint;
    else if (moveState == 1)
      setpoint = originalSetpoint - movingAngleOffset;
    else
      setpoint = originalSetpoint + movingAngleOffset;
}


//PID Ayarı (3 potansiyometre)

#if MANUAL_TUNING
void setPIDTuningValues()
{
    readPIDTuningValues();
    
    if (kp != prevKp || ki != prevKi || kd != prevKd)
    {
#if LOG_PID_CONSTANTS
      Serial.print("kp = ")  Serial.print(kp);Serial.print(", ");Serial.print("ki = ");Serial.print(ki);Serial.print("kd = ");Serial.print(", ");Serial.println(kd);
#endif

        pid.SetTunings(kp, ki, kd);
        prevKp = kp; prevKi = ki; prevKd = kd;
    }
}


void readPIDTuningValues()
{
    int potKp = analogRead(A0);
    int potKi = analogRead(A1);
    int potKd = analogRead(A2);
        
    kp = map(potKp, 0, 1023, 0, 25000) / 100.0; //0 - 250
    ki = map(potKi, 0, 1023, 0, 100000) / 100.0; //0 - 1000
    kd = map(potKd, 0, 1023, 0, 500) / 100.0; //0 - 5
    
}
#endif
