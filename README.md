# Denge-Robotu "Hacıyatmaz Robot"
Bu repoda MPU-6050 6 eksen gyro ve ivme sensörü kullanılarak yapılan dengede durabilme yeteneğine sahip robotun yapımı anlatılmıştır.

## Kullanılan Malzemeler

### 1- MPU6050 Ivme Ve Gyro Sensörü
MPU050 ivme sönsörü genellikle İHA ve denge robotu tarzı araçlarda kullanılan 3 eksenli açısal ivme ve 3 eksenli gyroskop ölçümü yapalıbal 6 eksenli sensörlerdir. İvme ölçümü bize hızdaki değişimi verirken, jiroskop ise bize açısal hızın korunmasından kaynaklı yönü vermektedir. Ayrıca, modül üzerinde X ve Y eksenlerini gösteren işaretler mevcuttur.
Sensör üzerinde toplanda 8 adet pin bulunmaktadır. VCC pini gücün + bacağına, GND pini ise - bacağına bağlanmalıdır. 3.3volt ile çalışmaktadır. 
SCL ve SDA pinleri ise I2C haberleşme protokolünü sağlamaktadır. I2C (Inter-Integrated Circuit) haberleşme protokolü, seri haberleşme türklerinden senkron haberleşmeyi sağlamaktadır. Bu protokol ile hızlı bir şekilde aktarım yapılmaktadır.
En sondaki INT pini ise programlama kartı üzerindeki bir dijital pine takılarak kullanılmaktadır.

### 2- Arduino Uno
Arduino Uno , ATmega328P tabanlı bir mikro denetleyici kartıdır. 14 dijital giriş/çıkış pinine (6 tanesi PWM çıkışı olarak kullanılabilir), 6 analog girişe, 16 MHz seramik rezonatöre (CSTCE16M0V53-R0), USB bağlantısına, güç jakına, ICSP başlığına ve sıfırlama düğmesine sahiptir.[1] İçine yazılım kurabilmek için kendi IDE'sine dahiptir. Bu linkten Arduino IDE'sini indirebilirsiniz.

### 3- DC Motorlar ve Motor Sürücü(L298n)
Arduino ile kullanılan bu DC motor ileri veya geri gidebilir ve dönüş hızı ayarlanabilir. Arduino'dan motorlara verilen akım motorları çalıştırmak için yeterli değildir.
Bu nedenle dc motorlar bir motor sürücüsü ile birlikte kullanılmak zorundadır. Bu proje de motor sürücü olarak iki motoru aynı anda sürebilen L298n Motor Sürücü kartı kullanılmaktadır.

### 4- Batarya
Arduino 9 voltluk pil ile beslenmektedir. Bunun yanı sıra motorlar da motorv sürücü yardımıyla (6*1,5v) 9voltluk gerilim ile beslenmektedir. Motor sürücüye gelen gerilimin arduinonun vIn ve GND pinlerine de bağlantısı yapılmaktadır.

### 5- Anahtar
Devreye gelen enerjinin açıp kapatılması için bir anahtar kullanılmaktadır.

## Devre Bağlantıları
![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Flowchart.jpg)
![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/sistem_dizayn.jpg)

### 1- Motor -> Motor Sürücü
İlk olarak motorlar motor sürücünün birer tarafına bağlanmaktadır. Yönlerinde eğer hata varsa en son motorun iki ucunu ters çevirevbilirsiniz!.

### 2- Motor Sürücü -> Arduino
Daha sonra motor sürcü arduinoya bağlanmaktadır. Bu çalışmada ENA ve ENB pinleri de kullanılmaktadır. Sırasıyla bağlantılar şu şekildedir. ENA ve ENB pinleri sırasıyla arduinonun 3. ve 6. pinine; IN1, IN2, IN3, IN4 pinleri ise sırasıyla arduinonun 4.,8.,5. ve 7. pinlerine bağlanmalıdır.

int ENA = 3;

int IN1 = 4;

int IN2 = 8;

int IN3 = 5;

int IN4 = 7;

int ENB = 6;

LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, 0.6, 1);

### 3- MPU6050 -> Arduino
Sonraki aşamada MPU6050'nin arduinoya bağlantısı yapılır. Sensörün VCC pini arduinonun 3.3 voltu ile beslenmektedir. GND pini yine GND pini ile bağlanmalıdır. I2C protokolü ile seri haberleşmeyi sağlayan SCL ve SDA pinleri de sırasıyla arduino üzerindeki SCL ve SDA pinleri ile eşlenmesi gerkekmektedir. Son olarak ise INT pini arduinonun dijial D1 pini ile bağlanmaktadır.

### 4- Enerji -> Devre
Son olarak bu robota bir enerji verilmesi gerekmektedir. Motor sürücünün 12V pinine pilin arttı ucu, GND pinine ise pilin eksi ucu bağlanmalıdır. Bu eksi GND bağlantısından aynı zamnada arduinonun da GND pinine bbir bağlantı yapılmalıdır. Motor sürücü üzerindeki 5V pini arduinonun VIN pinine bağlanmalıdır.

## Yapım Aşaması / Dizayn
Robotu iki katlı olarak tasarladım. Bu katlar 20cm-10cm boyutlarında bir profil keserek yapılmıştır. İlk kata motorların girmesi için girintiler açılmıştır. ve piller bu kata yerleştrilmiştir. İlk başta 9 volt olarak tasarlanan bu robot daha sonra 12 volta yükseltilmiştir. Arduino, motor sürücü ve MPU6050 ise üst katına yerleştirilmiştir. Ürünler yerleştirildikten sonra güzel durması açısından kablolar kısaltılarak lehimlenmiştir. son olarak ise robotun dışına bir kaplama yapılmıştır.

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/2.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/3.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/9.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/5.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/6.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/7.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/8.jpeg)

![alt text](https://github.com/Burakzdd/Denge-Robotu/blob/main/Pictures/10.png)
## Çalışma mantığı

Bu projede çalışırken dikkat edilmesi gereken en önemli durum motorların dönüş yönüdür. Robotu kendimize doğru eğdiğimizde motorların bize doğru hareket etmesi gerekmektedir. Robotun çalışma mantığı şu şekildedir. Robot her zaman kendini düz konuma getirmeye yani x eksenindeki eğimini sıfırlamaya çalışmaktadır. robot ileri doğru yattığında motorlar ileri giderek robotu düz konuma getirmeye çalışmaktadır. Aynı şekilde robot geriye doğru gittiğinde motorlar geriye doğru giderek robotun eğinimi sıfırlamaya çalışmaktadır
