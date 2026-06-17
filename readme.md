# DP Week 2 - Task 2 - Develop Your Own Core Loop
Nama : Muhammmad Syifaul Qolbi
NRP  : 5224600072
Kelas: GT 12-C

# Blackjack Roguelike — Design Pattern Week 2

Game roguelike berbasis Blackjack di mana pemain bertarung melawan dealer menggunakan tangan kartu. Setiap kemenangan memberikan damage ke dealer; kalah berarti pemain menerima damage. Setelah setiap ronde, pemain dapat membeli perk di toko untuk memperkuat serangan.

---

## Cara Build dan Jalankan

### Prasyarat
- Compiler C++17 atau lebih baru (g++ / clang++)

### Compile

```bash
g++ -std=c++17 -Iinclude src/Card.cpp src/Perk.cpp src/PerkFactory.cpp src/ScoringRule.cpp src/InputProvider.cpp src/Shop.cpp src/RunSession.cpp src/main.cpp -o game
```

### Jalankan

```bash
./game
```

---

## Struktur File

```
blackjack_roguelike/
├── include/                  # Header files (interface / deklarasi)
│   ├── Card.h                # Struct kartu dan fungsi utilitas
│   ├── Perk.h                # Abstract class Perk + implementasi konkret
│   ├── PerkFactory.h         # Factory untuk membuat perk
│   ├── ScoringRule.h         # Abstract class untuk kalkulasi damage (mutable)
│   ├── InputProvider.h       # Abstract class untuk input pemain (mutable)
│   ├── Shop.h                # Kelas toko antar ronde
│   └── RunSession.h          # Core loop invariant
│
├── src/                      # Source files (implementasi)
│   ├── Card.cpp
│   ├── Perk.cpp
│   ├── PerkFactory.cpp
│   ├── ScoringRule.cpp
│   ├── InputProvider.cpp
│   ├── Shop.cpp
│   ├── RunSession.cpp        # Urutan fase TIDAK boleh berubah
│   └── main.cpp              # Entry point — injeksi dependensi
│
└── README.md
```

---

## Urutan Fase (Invariant)

Urutan berikut adalah **inti dari game** dan tidak boleh diubah:

```
1. PlayerAction     — Pemain memilih Hit atau Stand
2. SystemEvaluate   — Dealer menarik kartu hingga total >= 17
3. ApplyDamage      — Damage dihitung via ScoringRule, HP diperbarui
4. UpdateState      — Cek kondisi menang/kalah (loop condition)
5. (Repeat)
```

Fase **antar ronde** (heal, reward, toko) bersifat mutable dan dieksekusi setelah dealer kalah, sebelum ronde berikutnya.

Jika urutan fase di atas diubah — misalnya `SystemEvaluate` diletakkan sebelum `PlayerAction` — maka dealer akan menarik kartu sebelum pemain sempat bermain, sehingga logika permainan menjadi tidak valid.

---

## Contoh Output

```
Tidak ada starting perk

====================
RONDE 1
HP Dealer: 17

HP Player: 63 | HP Dealer: 17
Kartu Kamu: 8(Heart) K(Spade)
Total: 18
Hit(h) Stand(s): s

=== HASIL HAND ===
Kartu Dealer: 6(Club) 9(Diamond) 5(Heart)
Total Dealer: 20
Kartu Kamu: 8(Heart) K(Spade)
Total Kamu: 18
Dealer Menang Hand! Kamu menerima 20 damage

HP Player: 43 | HP Dealer: 17
...

Ronde 1 selesai!
Sembuh 21 HP
HP saat ini: 64
Mendapat 15 gold
Total gold: 15

===== SHOP =====
Cash: 15

1. +21% Damage (30 gold)
2. +5 Flat Damage (25 gold)
0. Skip
Pilihan: 0
Lewati toko
```

---

## Penjelasan Modifikasi

Dibandingkan kode awal, terdapat tiga perubahan arsitektur utama:

### 1. `ScoringRule` — memisahkan logika damage dari `RunSession`

Sebelumnya, `RunSession` langsung menghitung damage dengan cara iterasi perk secara manual. Sekarang logika itu dipindahkan ke `ScoringRule`:

```cpp
// Sebelum (di dalam RunSession):
int damage = playerTotal;
for (Perk* p : perks)
    damage = p->modifyDamage(damage);

// Sesudah — RunSession hanya memanggil:
int damage = m_scoring->calculateDamage(playerTotal, perks);
```

`StandardScoringRule` adalah implementasi default, tetapi bisa diganti (misal `MultiplicativeScoringRule`) tanpa menyentuh `RunSession`.

### 2. `InputProvider` — memisahkan sumber input dari logika game

Sebelumnya `RunSession` langsung membaca `std::cin`. Sekarang semua input melalui abstraksi:

```cpp
char choice = m_input->getHitOrStand();
int  shop   = m_input->getShopChoice();
```

Ini memungkinkan `ConsoleInputProvider` diganti dengan `AIInputProvider` atau `TestInputProvider` untuk keperluan pengujian otomatis.

### 3. Fase loop dibuat eksplisit sebagai method private

Empat fase (`playerAction`, `systemEvaluate`, `applyDamage`, `isRoundOver`) dijadikan method terpisah sehingga kode `run()` mencerminkan langsung urutan invariant, bukan tercampur dengan implementasi detail.

---

## Refleksi

### 1. Apa struktur invariant dari game ini?

Invariant adalah bagian dari program yang **tidak boleh berubah urutan atau keberadaannya** tanpa merusak fungsi dasar game. Dalam program ini, invariant adalah urutan fase dalam loop utama `RunSession::run()`:

1. `playerAction` — pemain harus bertindak lebih dulu
2. `systemEvaluate` — dealer merespons setelah pemain selesai
3. `applyDamage` — damage baru bisa dihitung setelah kedua pihak selesai bermain
4. Cek kondisi menang/kalah — hanya valid setelah damage diterapkan

Selain urutan, keberadaan `RunSession`, `Card`, dan `Perk` sebagai antarmuka juga merupakan invariant — game tidak bisa berjalan tanpa ketiga komponen ini.

### 2. Bagian mana yang bersifat mutable?

| Komponen | Alasan Mutable |
|---|---|
| `ScoringRule` | Formula damage bisa diganti tanpa mengubah loop |
| `InputProvider` | Sumber input bisa berupa konsol, AI, atau data uji |
| `Perk` (implementasi konkret) | Perk baru bisa ditambah tanpa mengubah kelas lain |
| `PerkFactory` | Tipe perk yang tersedia bisa diubah bebas |
| `Shop` | Konten toko (harga, pilihan) bisa diubah tanpa menyentuh loop |
| `dealerHP = 12 + round * 5` | Formula scaling HP dealer bisa diubah |
| `reward = 10 + round * 5` | Formula reward bisa diubah |
| `playerHP awal = 63` | Nilai awal bisa disesuaikan kesulitan |

### 3. Jika ingin menambah fitur baru, kelas mana yang berubah?

Tergantung fiturnya:
- **Perk baru** → tambah subclass baru di `Perk.h/cpp`, daftarkan di `PerkFactory`. `RunSession` tidak perlu disentuh.
- **Formula damage baru** → buat subclass baru dari `ScoringRule`, inject ke `main.cpp`. `RunSession` tidak berubah.
- **Item baru di toko** → ubah `Shop.cpp` dan `PerkFactory`. Loop utama tidak berubah.
- **Input dari AI** → buat `AIInputProvider` yang implement `InputProvider`. Ganti di `main.cpp`. `RunSession` tidak berubah.

Inilah tujuan dari pemisahan kelas — fitur baru menambah kode baru, bukan mengubah kode lama.

### 4. Apa yang terjadi jika urutan loop diubah?

- Jika `systemEvaluate` dipindah sebelum `playerAction` → dealer menarik kartu sebelum pemain bermain, sehingga kartu dealer sudah final sebelum pemain bisa bereaksi. Logika permainan rusak.
- Jika `applyDamage` dipindah sebelum `systemEvaluate` → damage dihitung dari total dealer yang belum final. Hasil tidak valid.
- Jika cek menang/kalah dipindah sebelum `applyDamage` → HP tidak pernah berubah karena damage belum diterapkan, game tidak bisa berakhir.

Urutan fase bukan sekadar konvensi — tiap fase bergantung pada hasil fase sebelumnya.
