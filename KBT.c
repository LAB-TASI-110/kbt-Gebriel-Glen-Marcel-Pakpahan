#include <stdio.h> // Header untuk fungsi input/output standar seperti printf dan fgets.
#include <string.h> // Header untuk fungsi manipulasi string seperti strlen, strcspn, strcmp, strcasecmp.
#include <stdlib.h> // Header untuk fungsi utilitas umum seperti abs (nilai absolut) dan exit (keluar program).
#include <ctype.h>  // Header untuk fungsi pengujian karakter seperti isalpha (cek alfabet).

// --- FUNGSI UTILITY ---
// Sebuah bagian khusus untuk fungsi-fungsi pembantu yang digunakan di program utama.

// Fungsi untuk membersihkan buffer input.
// Ini diperlukan karena fgets/scanf dapat meninggalkan karakter newline ('\n') di buffer,
// yang bisa mengganggu input selanjutnya.
void clearInputBuffer() {
    int c; // Deklarasi variabel integer untuk menampung karakter.
    // Loop ini akan membaca dan membuang karakter dari buffer input
    // sampai menemukan newline ('\n') atau end-of-file (EOF).
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi untuk validasi nama.
// Memastikan bahwa nama hanya mengandung karakter alfabet (huruf) dan spasi.
int isValidName(const char *name) {
    // Loop melalui setiap karakter dalam string 'name'.
    // name[i] != '\0' adalah kondisi untuk berhenti saat mencapai akhir string (null terminator).
    for (int i = 0; name[i] != '\0'; i++) {
        // Memeriksa apakah karakter saat ini BUKAN alfabet DAN BUKAN spasi.
        // !isalpha(name[i]) menguji apakah karakter bukan huruf.
        // name[i] != ' ' menguji apakah karakter bukan spasi.
        // Jika keduanya benar, berarti ada karakter yang tidak diizinkan.
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0; // Mengembalikan 0 (false) jika ditemukan karakter tidak valid.
        }
    }
    return 1; // Mengembalikan 1 (true) jika semua karakter valid.
}

// Fungsi serbaguna untuk mendapatkan input string dari pengguna dengan validasi.
// prompt: Pesan yang ditampilkan ke pengguna.
// buffer: Pointer ke array karakter tempat input akan disimpan.
// bufferSize: Ukuran maksimum buffer untuk mencegah buffer overflow.
// validator: Pointer ke fungsi validasi kustom (bisa NULL jika tidak ada validasi khusus).
void getInput(const char *prompt, char *buffer, int bufferSize, int (*validator)(const char *)) {
    while (1) { // Loop tak terbatas sampai input yang valid diterima.
        printf("%s: ", prompt); // Menampilkan prompt ke pengguna.
        // fgets membaca input dari stdin (keyboard) hingga newline atau bufferSize-1 karakter.
        // Menyimpan hasil di 'buffer'. Mengembalikan NULL jika ada error.
        if (fgets(buffer, bufferSize, stdin) != NULL) {
            // strcspn mencari posisi karakter newline ('\n') di buffer.
            // Jika '\n' ditemukan, ia diganti dengan null terminator ('\0') untuk menghapus newline.
            buffer[strcspn(buffer, "\n")] = 0; 

            // Memeriksa apakah input kosong.
            if (strlen(buffer) == 0) {
                printf("Input tidak boleh kosong. Mohon masukkan nilai.\n");
                continue; // Lanjut ke iterasi loop berikutnya untuk meminta input lagi.
            }

            // Memeriksa apakah ada fungsi validator yang diberikan DAN input tidak valid menurut validator.
            if (validator != NULL && !validator(buffer)) {
                printf("Input tidak valid. Mohon periksa kembali formatnya.\n");
                continue; // Lanjut ke iterasi loop berikutnya.
            }
            break; // Keluar dari loop jika input valid.
        } else {
            printf("Error saat membaca input. Mohon coba lagi.\n"); // Pesan error jika fgets gagal.
            clearInputBuffer(); // Membersihkan buffer jika ada masalah input.
        }
    }
}

// --- PROGRAM UTAMA ---
// Titik awal eksekusi program.
int main() {
    // Definisi array konstanta untuk kode angkot yang valid.
    const char *validAngkotCodes[] = {"17", "08", "07", "05", "10", "06"};
    // Menghitung jumlah elemen dalam array validAngkotCodes.
    int numValidAngkotCodes = sizeof(validAngkotCodes) / sizeof(validAngkotCodes[0]);

    // Definisi array konstanta untuk daftar kabupaten di Sumatera Utara.
    const char *kabupatenSumut[] = {
        "Deli Serdang", "Langkat", "Karo", "Simalungun", "Asahan",
        "Labuhanbatu", "Tapanuli Utara", "Tapanuli Selatan", "Mandailing Natal", "Nias"
        // Anda dapat menambahkan lebih banyak kabupaten di sini sesuai kebutuhan.
    };
    // Menghitung jumlah elemen dalam array kabupatenSumut.
    int numKabupaten = sizeof(kabupatenSumut) / sizeof(kabupatenSumut[0]);

    // Deklarasi variabel untuk menyimpan data input dari pengguna.
    char nama[50];         // Buffer untuk nama (maksimal 49 karakter + null terminator).
    char asalDaerah[50];   // Buffer untuk asal daerah.
    char tujuanDaerah[50]; // Buffer untuk tujuan daerah.
    char kodeAngkot[10];   // Buffer untuk kode angkot.
    int hargaTiket = 0;    // Variabel untuk menyimpan harga tiket, diinisialisasi 0.
    int jarakRelatif;      // Variabel untuk menyimpan perhitungan jarak relatif antar daerah.

    // Menampilkan pesan selamat datang dengan format tebal (bold) menggunakan kode ANSI escape sequence.
    printf("\033[1mSILAHKAN PESAN TICKET\033[0m\n\n"); 

    // --- PROSES INPUT PENGGUNA ---

    // Input Nama: Menggunakan fungsi getInput dengan validator isValidName.
    getInput("NAMA", nama, sizeof(nama), isValidName);

    // Input Asal Daerah: Menggunakan loop untuk validasi khusus daftar kabupaten.
    while (1) {
        getInput("ASAL DAERAH", asalDaerah, sizeof(asalDaerah), NULL); // Tidak ada validator karakter khusus.
        int found = 0; // Flag untuk menandai apakah daerah ditemukan.
        // Loop untuk mencari asalDaerah dalam daftar kabupatenSumut.
        for (int i = 0; i < numKabupaten; i++) {
            // strcasecmp membandingkan dua string tanpa memperhatikan huruf besar/kecil.
            // Mengembalikan 0 jika string sama.
            if (strcasecmp(asalDaerah, kabupatenSumut[i]) == 0) { 
                found = 1; // Daerah ditemukan.
                break;     // Keluar dari loop pencarian.
            }
        }
        if (!found) { // Jika daerah tidak ditemukan.
            printf("Kabupaten '%s' tidak terdaftar di Sumatera Utara. Mohon masukkan nama kabupaten yang valid.\n", asalDaerah);
        } else {
            break; // Keluar dari loop input asal daerah jika valid.
        }
    }

    // Input Tujuan Daerah: Menggunakan loop untuk validasi daftar kabupaten dan mencegah asal = tujuan.
    while (1) {
        getInput("TUJUAN DAERAH", tujuanDaerah, sizeof(tujuanDaerah), NULL); // Tidak ada validator karakter khusus.
        int found = 0; // Flag untuk menandai apakah daerah ditemukan.
        // Loop untuk mencari tujuanDaerah dalam daftar kabupatenSumut.
        for (int i = 0; i < numKabupaten; i++) {
            if (strcasecmp(tujuanDaerah, kabupatenSumut[i]) == 0) {
                found = 1; // Daerah ditemukan.
                break;
            }
        }
        if (!found) { // Jika daerah tidak ditemukan.
            printf("Kabupaten '%s' tidak terdaftar di Sumatera Utara. Mohon masukkan nama kabupaten yang valid.\n", tujuanDaerah);
        } else if (strcasecmp(asalDaerah, tujuanDaerah) == 0) { // Jika asal dan tujuan sama.
            printf("Kabupaten asal dan tujuan tidak boleh sama. Mohon masukkan tujuan yang berbeda.\n");
        }
        else {
            break; // Keluar dari loop input tujuan daerah jika valid.
        }
    }
    
    // Input Kode Angkot: Menggunakan loop untuk validasi daftar kode angkot yang tersedia.
    while (1) {
        getInput("KODE ANGKOT", kodeAngkot, sizeof(kodeAngkot), NULL); // Tidak ada validator karakter khusus.
        int valid = 0; // Flag untuk menandai apakah kode angkot valid.
        // Loop untuk mencari kodeAngkot dalam daftar validAngkotCodes.
        for (int i = 0; i < numValidAngkotCodes; i++) {
            // strcmp membandingkan dua string (case-sensitive).
            if (strcmp(kodeAngkot, validAngkotCodes[i]) == 0) {
                valid = 1; // Kode angkot valid.
                break;
            }
        }
        if (!valid) { // Jika kode angkot tidak valid.
            printf("Kode angkot '%s' tidak valid. Kode yang tersedia: 17, 08, 07, 05, 10, 06. Mohon coba lagi.\n", kodeAngkot);
        } else {
            break; // Keluar dari loop input kode angkot jika valid.
        }
    }

    // --- PERHITUNGAN HARGA TIKET ---
    // Logika perhitungan harga tiket berdasarkan jarak relatif antar kabupaten.
    // Diasumsikan harga dasar 50.000 untuk jarak relatif 1,
    // dan bertambah 50.000 untuk setiap unit jarak relatif tambahan.
    
    int indexAsal = -1, indexTujuan = -1; // Inisialisasi indeks asal dan tujuan.
    // Loop untuk menemukan indeks (posisi) asal dan tujuan daerah dalam array kabupatenSumut.
    for (int i = 0; i < numKabupaten; i++) {
        if (strcasecmp(asalDaerah, kabupatenSumut[i]) == 0) {
            indexAsal = i; // Simpan indeks asal daerah.
        }
        if (strcasecmp(tujuanDaerah, kabupatenSumut[i]) == 0) {
            indexTujuan = i; // Simpan indeks tujuan daerah.
        }
    }

    // Memastikan kedua indeks ditemukan sebelum menghitung harga.
    if (indexAsal != -1 && indexTujuan != -1) {
        jarakRelatif = abs(indexTujuan - indexAsal); // abs() dari stdlib.h menghitung nilai absolut dari selisih indeks.
        if (jarakRelatif == 0) { // Kondisi ini seharusnya sudah dicegah di validasi input tujuan.
            hargaTiket = 0; // Jika jarak 0, harga 0.
        } else {
            // Formula harga: 50.000 (harga dasar) + (jarak_relatif - 1) * 50.000 (tambahan per unit jarak).
            // Contoh: jarakRelatif = 1 -> 50.000 + 0 * 50.000 = 50.000
            // Contoh: jarakRelatif = 2 -> 50.000 + 1 * 50.000 = 100.000
            hargaTiket = 50000 + (jarakRelatif - 1) * 50000;
        }
    } else {
        // Pesan error jika salah satu daerah tidak ditemukan (meskipun seharusnya sudah divalidasi).
        printf("Error: Kabupaten asal atau tujuan tidak ditemukan dalam daftar internal. Harga tidak dapat dihitung.\n");
        hargaTiket = 0; // Set harga tiket ke 0 sebagai default error.
    }

    // --- OUTPUT RINCIAN TIKET ---
    // Menampilkan rincian tiket yang telah dipesan.
    printf("\n--- Rincian Tiket Koperasi Bintang Tapanuli ---\n");
    printf("Nama Penumpang   : %s\n", nama);         // Menampilkan nama penumpang.
    printf("Asal Daerah      : %s\n", asalDaerah);   // Menampilkan asal daerah.
    printf("Tujuan Daerah    : %s\n", tujuanDaerah); // Menampilkan tujuan daerah.
    printf("Kode Angkot      : %s\n", kodeAngkot);   // Menampilkan kode angkot.
    printf("Harga Tiket      : Rp %d\n", hargaTiket); // Menampilkan harga tiket.
    printf("-----------------------------------------------\n");

    return 0; // Mengembalikan 0 menandakan program berakhir dengan sukses.
}