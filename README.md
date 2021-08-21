# Denge-Robotu
Bu repoda MPU-6050 6 eksen gyro ve ivme sensörü kullanılarak yapılan dengede durabilme yeteneğine sahip robotun yapımı anlatılmıştır.

## Kullanılan Malzemeler

### 1- MPU6050 Ivme Ve Gyro Sensörü
MPU050 ivme sönsörü genellikle İHA ve denge robotu tarzı araçlarda kullanılan 3 eksenli açısal ivme ve 3 eksenli gyroskop ölçümü yapalıbal 6 eksenli sensörlerdir. İvme ölçümü bize hızdaki değişimi verirken, jiroskop ise bize açısal hızın korunmasından kaynaklı yönü vermektedir. Ayrıca, modül üzerinde X ve Y eksenlerini gösteren işaretler mevcuttur.
![alt text]()
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

## Yapım Aşaması / Dizayn

