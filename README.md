# Denge-Robotu
Bu repoda MPU-6050 6 eksen gyro ve ivme sensörü kullanılarak yapılan dengede durabilme yeteneğine sahip robotun yapımı anlatılmıştır.

# Kullanılan Malzemeler

## 1- MPU6050 Ivme Ve Gyro Sensörü
MPU050 ivme sönsörü genellikle İHA ve denge robotu tarzı araçlarda kullanılan 3 eksenli açısal ivme ve 3 eksenli gyroskop ölçümü yapalıbal 6 eksenli sensörlerdir. İvme ölçümü bize hızdaki değişimi verirken, jiroskop ise bize açısal hızın korunmasından kaynaklı yönü vermektedir. Ayrıca, modül üzerinde X ve Y eksenlerini gösteren işaretler mevcuttur.

Kullanımı:
Sensör üzerinde toplanda 8 adet pin bulunmaktadır. VCC pini gücün + bacağına, GND pini ise - bacağına bağlanmalıdır. 3.3volt ile çalışmaktadır. 
SCL ve SDA pinleri ise I2C haberleşme protokolünü sağlamaktadır. I2C (Inter-Integrated Circuit) haberleşme protokolü, seri haberleşme türklerinden senkron haberleşmeyi sağlamaktadır. Bu protokol ile hızlı bir şekilde aktarım yapılmaktadır.
En sondaki INT pini ise programlama kartı üzerindeki bir dijital pine takılarak kullanılmaktadır.
