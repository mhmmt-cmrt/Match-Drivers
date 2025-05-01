# 🚖 Driver Matching System

Bu proje, sürücüleri yolcu talepleriyle eşleştiren basit bir **sürücü eşleştirme algoritması** içerir. Sistem; sürücü uygunluğu, mesafe, puan, iptal oranı, araç tipi, yolcu kapasitesi gibi çeşitli kriterleri göz önünde bulundurarak en uygun sürücüyü seçer.

## 🧠 Özellikler

- 📍 Mesafeye dayalı sürücü filtreleme
- 🚗 Araç tipi ve yolcu kapasitesi kontrolü
- ⭐ Sürücü puanı ve iptal oranı değerlendirmesi
- ⚠️ Kara listeye alınmış sürücülerin hariç tutulması
- 🔁 Surge bölgesine göre puanlama cezası
- 🎲 Eşit puanlı sürücüler arasında rastgele seçim
- 📊 Detaylı sürücü değerlendirme çıktısı (terminalde)

## 📦 Derleme ve Çalıştırma

```bash
g++ -std=c++17 -o match_driver main.cpp
./match_driver

```

## 📝 Giriş Bilgileri
Program çalıştırıldığında aşağıdaki bilgileri girmeniz istenir:

- 🚩 Pickup Latitude & Longitude: Yolcunun bulunduğu konum
- 🚘 Vehicle Type: Gerekli araç tipi (standard veya premium)
- 🌟 Minimum Driver Rating: Kabul edilen en düşük sürücü puanı
- 🧍‍♀️ Passenger Count: Toplam yolcu sayısı
- ⚡ Surge Zone: Fiyat artış bölgesi mi? (yes veya no)
  
## 📌 Eşleşme Kriterleri
Aşağıdaki koşulları sağlamayan sürücüler filtrelenir:

- Müsait değilse
- Aktif bir yolculuğu varsa
- Kara listedeyse
- Uygun araç tipine sahip değilse
- Minimum puanın altındaysa
- Yolcu kapasitesini aşıyorsa
- 10 km'den uzaktaysa

## 📈 Puanlama Sistemi
Her uygun sürücü için aşağıdaki formül uygulanır:


- finalScore = ETA * surgePenalty + cancelScore * 15.0 - rating
- ETA: Tahmini varış süresi
- surgePenalty: Surge bölgelerinde 1.2, aksi takdirde 1.0
- cancelScore: Normalize edilmiş iptal oranı
- rating: Sürücü puanı (daha düşük skor daha iyidir)

En düşük skora sahip sürücü seçilir. Birden fazla sürücü aynı skoru alırsa rastgele biri atanır.

📂 Dosya Yapısı

.
├── main.cpp        # Sürücü eşleştirme algoritması
└── README.md       # Açıklama dosyası

##  🔧 Geliştirme İpuçları

- Daha gerçekçi mesafe hesaplaması için Haversine formülü entegre edilebilir.
- Sürücü geçmişi ve yolcu geri bildirimi puanlamaya dahil edilebilir.
- Kullanıcı arayüzü (GUI veya web) ile desteklenebilir.

##  📄 Lisans
Bu proje açık kaynak olup MIT lisansı ile sunulmaktadır.







