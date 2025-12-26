Berikut adalah analisis mendalam mengenai Source Code "Big Challenge Word Counter" kamu, beserta file `README.md` yang siap pakai untuk GitHub.

---

### 🔎 Analisis Code: Pro & Kontra

Secara keseluruhan, kode ini **sangat solid** untuk memenuhi spesifikasi tugas kuliah. Kamu menggunakan pendekatan "aman" (Static Memory) dan algoritma parsing yang detail (State Machine). Namun, ada beberapa sisi teknis yang bisa ditingkatkan untuk performa industri.

#### ✅ Kelebihan (Pros)

1. **Arsitektur Modular yang Bersih:**
* Pemisahan kode menjadi `main`, `process` (logika bisnis), dan `fileio` (input/output) sangat baik. Ini memudahkan *debugging* dan *maintenace*.
* Penggunaan Header Guard (`#ifndef`) sudah tepat.


2. **HTML Parsing yang Sangat Kuat (State Machine):**
* Ini adalah bagian terbaik kodemu. Kamu menggunakan `fgetc` dengan *flag* (`inTitle`, `inBody`, `inUrl`, `inSkip`).
* **Kenapa ini bagus?** Parser ini tahan banting. Meskipun file HTML ditulis dalam satu baris panjang atau ribuan baris acak, parser tetap bisa membedakan mana teks berita dan mana kode CSS/Script.


3. **Manajemen Memori yang "Aman" (Untuk Pemula):**
* Menggunakan *Static Array* (`AlphabetGroup words[26]` dan `MAX_WORDS_GROUP`) menghilangkan risiko *Memory Leak* karena tidak ada `malloc`/`free`. Program sangat stabil.


4. **Validasi Input yang Baik:**
* Pembersihan buffer (`while (getchar() != '\n')`) setelah `scanf` mencegah program *infinite loop* saat user salah input (misal memasukkan huruf saat diminta angka).
* Validasi file exist/tidak sebelum diproses.


5. **Kepatuhan pada Spesifikasi:**
* Algoritma *Insertion Sort* diimplementasikan dengan benar (meskipun lambat, ini sesuai permintaan tugas).
* Logika sorting (`Freq` > `Len` > `Alpha`) sudah tepat.



#### ❌ Kekurangan / Kelemahan (Cons)

1. **Boros Memori (Inefficiency):**
* Kamu mengalokasikan `MAX_WORDS_GROUP` (15.000) untuk *setiap* abjad.
* Hitungan kasar: 26 huruf * 15.000 kata * 54 bytes (struct) ≈ **21 MB**.
* Untuk huruf seperti 'Q' atau 'X' yang katanya sedikit, kamu membuang puluhan MB memori kosong yang tidak terpakai.


2. **Hard Limit (Keterbatasan Data):**
* Jika ada huruf (misal 'A' atau 'S') yang memiliki lebih dari 15.000 kata unik dalam data input, kata ke-15.001 dan seterusnya akan **dibuang** (karena `if (group->count >= MAX_WORDS_GROUP) return;`). Ini bisa membuat hasil perhitungan tidak 100% akurat untuk data masif.


3. **Performa Sorting ():**
* *Insertion Sort* sangat lambat untuk data besar (50.000 kata). Program akan terasa "hang" beberapa detik saat memproses data. (Tapi ini dimaafkan karena syarat tugas meminta *simple sort*).


4. **Keamanan Input Filename:**
* `scanf("%s", filename);` berbahaya. Jika user memasukkan nama file lebih dari 50 karakter, akan terjadi *Buffer Overflow* yang bisa menimpa memori lain. Sebaiknya gunakan `fgets` atau batasi lebar input `scanf("%49s", filename)`.


5. **Ukuran File Binary Besar:**
* Kamu menyimpan `struct` mentah-mentah (`fwrite`). Karena `char word[50]` ukurannya tetap, kata "aku" (3 byte) tetap memakan ruang 50 byte di hardisk. File `.bin` akan jauh lebih besar dari yang seharusnya.



---

### 📝 README.md (GitHub Interactive Style)

Berikut adalah file `README.md` yang lengkap, profesional, dan menggunakan format Markdown modern. Copy kode di bawah ini dan simpan sebagai `README.md` di folder root project kamu.

```markdown
# 📊 Big Challenge Word Counter

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="Language C">
  <img src="https://img.shields.io/badge/Type-Console%20Application-4EAA25?style=for-the-badge" alt="Console App">
  <img src="https://img.shields.io/badge/Algorithm-Insertion%20Sort-orange?style=for-the-badge" alt="Algorithm">
  <img src="https://img.shields.io/badge/Status-Completed-success?style=for-the-badge" alt="Status">
</p>

<p align="center">
  <b>Aplikasi Analisis Frekuensi Kata dari Dokumen HTML/Teks Mentah</b><br>
  Project Akhir Semester Pemrograman Dasar (SINF-1001)
</p>

---

## 📖 Deskripsi

**Big Challenge Word Counter** adalah program berbasis C yang dirancang untuk memproses file teks besar (seperti dump artikel berita), membersihkan tag HTML, dan menghitung frekuensi kemunculan setiap kata.

Program ini menggunakan teknik **State Machine Parsing** untuk memisahkan konten yang relevan (`<title>`, `<body>`) dari elemen teknis (`<script>`, `<style>`, `<url>`). Hasil analisis diurutkan berdasarkan frekuensi tertinggi dan disimpan dalam format biner untuk akses cepat.

### ✨ Fitur Utama
* 🚀 **Smart Parsing:** Mengabaikan tag HTML, CSS, JavaScript, dan URL secara otomatis.
* 🧹 **Data Cleaning:** Membersihkan tanda baca dan mengubah teks menjadi *lowercase*.
* blacklist **Filter:** Membuang kata-kata umum internet (seperti `www`, `https`, `com`).
* 📊 **Sorting Cerdas:** Mengurutkan data berdasarkan:
    1.  Frekuensi (Terbanyak → Sedikit)
    2.  Panjang Kata (Terpanjang → Terpendek)
    3.  Abjad (A → Z)
* 💾 **Binary Storage:** Menyimpan hasil olahan ke file `.bin` agar tidak perlu parsing ulang.
* 📈 **Top-N View:** Menampilkan N-kata teratas per abjad dalam format tabel rapi.

---

## 📂 Struktur Project


```

📦 bigchallenge
┣ 📂 fileio
┃ ┣ 📜 fileio.c       # Menangani Input/Output File & Menu
┃ ┗ 📜 fileio.h       # Header File I/O
┣ 📂 process
┃ ┣ 📜 process.c      # Logika Parsing, Cleaning, & Sorting
┃ ┗ 📜 process.h      # Definisi Struct & Konstanta
┣ 📜 main.c           # Entry point program
┣ 📜 Data-50K.txt     # Contoh data input (Raw HTML/Text)
┗ 📜 makefile         # Script kompilasi (Opsional)

```

---

## ⚙️ Instalasi & Kompilasi

Pastikan kamu memiliki compiler GCC (`MinGW` untuk Windows atau `build-essential` untuk Linux).

### 1. Clone Repository
```bash
git clone [https://github.com/username-anda/bigchallenge.git](https://github.com/username-anda/bigchallenge.git)
cd bigchallenge

```

### 2. Kompilasi Kode

Jalankan perintah berikut di terminal:

```bash
gcc -Wall main.c process/process.c fileio/fileio.c -o bigchallange

```

---

## 🚀 Cara Penggunaan

1. Jalankan program yang sudah dikompilasi:
* **Windows:** `.\bigchallange.exe`
* **Linux/Mac:** `./bigchallange`


2. Akan muncul menu utama:
```text
=================================================
|             WORD COUNTER PROGRAM              |
================================================|
| 1. Baca Data                                  |
| 2. Menampilkan n kata & Baca Data (jika Belum)|
| 3. Keluar                                     |
=================================================
Pilih >> 

```


3. **Pilih Menu 1** untuk memproses data mentah pertama kali. Masukkan nama file (contoh: `Data-50K.txt`). Tunggu hingga proses sorting selesai.
4. **Pilih Menu 2** untuk melihat hasil tabel statistik kata. Masukkan jumlah kata yang ingin ditampilkan per huruf (misal: 5).

---

## 🧠 Detail Teknis

### Struktur Data

Program ini menggunakan *Static Array of Struct* untuk manajemen memori yang stabil tanpa fragmentasi.

```c
typedef struct {
    char word[50];
    int frequency;
} WordEntry;

typedef struct {
    WordEntry entries[15000]; // Menampung hingga 15k kata unik per huruf
    int count;
} AlphabetGroup;

```

### Algoritma Pengurutan

Sesuai spesifikasi tugas, program menggunakan **Insertion Sort**. Meskipun kompleksitas waktunya , algoritma ini stabil dan mudah diimplementasikan untuk struktur data array.

### Logika Parser

Menggunakan pendekatan karakter-per-karakter (`fgetc`) untuk mendeteksi konteks:

* `inTitle` / `inBody`: ✅ Ambil Kata
* `inUrl` / `inScript` / `inStyle`: ❌ Abaikan/Skip

---

## 📸 Preview Output

**Tampilan Tabel Statistik:**

```text
abjad {kata (frekuensi)}

a     {akan (90146), ada (31944), asal (23172), adalah (23169), atas (15216)}
b     {bisa (54470), belum (24550), bermain (23797), baru (20707), baik (20485)}
c     {chelsea (12302), city (11430), cedera (11391), cukup (10733), champions (7930)}
...
z     {zimbio (3049), zona (1409), zlatan (445), zanetti (434), zidane (319)}

```

---

## ⚠️ Batasan (Limitations)

* **Memori:** Membutuhkan RAM sekitar ~25MB karena penggunaan array statis besar.
* **Kapasitas:** Maksimal menampung 15.000 kata unik per abjad. Jika data input memiliki variasi kata yang sangat ekstrem, kata berlebih akan diabaikan.
* **Kecepatan:** Proses sorting mungkin memakan waktu beberapa detik untuk data di atas 5MB.

---

## 👨‍💻 Author

**Muhammad Aqil Mubarak**
**Ahmad Damanhuri**

* Mahasiswa Informatika - Universitas Syiah Kuala
* Project Big Challenge 2025/2026

---

<p align="center">Made with ❤️ and C</p>

```

fix this readme made it all in one in file .md
