# Panduan Instalasi Lengkap live-app

Dokumen ini akan memandu Anda melalui proses instalasi `live-app` dan semua dependensinya di komputer Windows baru.

## Langkah 1: Instalasi Alat Pengembangan (Visual Studio)

`live-app` dikembangkan menggunakan C++ dan C#, jadi Anda memerlukan Visual Studio.

1.  **Unduh Visual Studio Installer**: Kunjungi [situs web Visual Studio](https://visualstudio.microsoft.com/downloads/) dan unduh "Visual Studio Community 2022". Versi Community gratis untuk proyek sumber terbuka dan pengembang individu.
2.  **Instal Workloads**: Jalankan installer. Pada tab "Workloads", pilih dua komponen berikut:
    *   **`.NET desktop development`**: Diperlukan untuk bagian C# (antarmuka pengguna).
    *   **`Desktop development with C++`**: Diperlukan untuk bagian C++ (mesin inti). Pastikan di panel kanan, **`C++/CLI support for v143 build tools`** juga dicentang.
3.  **Mulai Instalasi**: Klik "Install" dan tunggu hingga proses selesai.

## Langkah 2: Instalasi GStreamer SDK

GStreamer adalah kerangka kerja multimedia yang menjadi inti dari `live-app`.

1.  **Unduh GStreamer**: Buka [halaman unduhan GStreamer](https://gstreamer.freedesktop.org/download/).
2.  **Pilih Versi yang Tepat**: Navigasikan ke bagian **Windows**. Unduh versi **MSVC 64-bit (VS 2019, C++)**. Pilih versi "development" dan "runtime" installer.
3.  **Jalankan Installer**:
    *   Jalankan installer **development**. Pilih instalasi "Complete".
    *   Jalankan installer **runtime**. Pilih instalasi "Typical".
    *   **Penting**: Selama instalasi, pastikan Anda menginstal GStreamer ke direktori default (`C:\gstreamer\1.0\msvc_x86_64`). Kode proyek saat ini mengasumsikan lokasi ini.

## Langkah 3: Instalasi OBS Virtual Cam

`live-app` menggunakan OBS Virtual Cam untuk membuat output kamera virtual.

1.  **Unduh OBS Virtual Cam**: Buka [halaman rilis OBS Virtual Cam](https://github.com/miaulightouch/obs-virtual-cam/releases).
2.  **Pilih Versi Terbaru**: Unduh file `obs-virtualcam-x.x.x-windows-x64-installer.exe` terbaru.
3.  **Jalankan Installer**: Jalankan installer dan ikuti petunjuk di layar.

## Langkah 3: Dapatkan Kode Sumber `live-app`

Anda bisa mendapatkan kode dengan mengkloning repositori Git atau mengunduh sebagai file ZIP.

*   **Menggunakan Git (Disarankan)**:
    1.  Instal [Git for Windows](https://git-scm.com/download/win).
    2.  Buka Command Prompt atau PowerShell dan jalankan perintah berikut:
        ```bash
        git clone https://github.com/your-username/live-app.git
        ```
        (Ganti `your-username` dengan nama pengguna Anda yang sebenarnya jika Anda telah membuat fork).
*   **Mengunduh ZIP**:
    1.  Buka halaman repositori di GitHub.
    2.  Klik tombol hijau "Code", lalu pilih "Download ZIP".
    3.  Ekstrak file ZIP ke lokasi yang Anda inginkan.

## Langkah 4: Membangun (Compile) Proyek

Sekarang setelah semua dependensi terinstal dan kode sumber tersedia, Anda dapat membangun aplikasi.

1.  **Buka Solusi**: Navigasikan ke direktori `live-app` dan buka file `live-app.sln` dengan Visual Studio 2022.
2.  **Atur Konfigurasi Build**: Di toolbar atas Visual Studio, atur konfigurasi build ke **`Release`** dan platform ke **`x64`**.
3.  **Bangun Solusi**:
    *   Klik kanan pada `Solution 'live-app'` di Solution Explorer.
    *   Pilih **`Build Solution`**.
    *   Visual Studio akan mengkompilasi proyek C++ dan C#. Anda dapat melihat kemajuan di jendela "Output".

## Langkah 5: Menjalankan Aplikasi

Setelah proses build berhasil, Anda dapat menjalankan `live-app`.

*   **Dari Visual Studio**:
    *   Klik kanan pada proyek `LiveApp.UI.CSharp` di Solution Explorer.
    *   Pilih `Debug` -> `Start Without Debugging` (atau tekan `Ctrl+F5`).
*   **Langsung dari File Explorer**:
    *   Navigasikan ke `live-app\src\LiveApp.UI.CSharp\bin\x64\Release\net6.0-windows\`.
    *   Jalankan file `LiveApp.UI.CSharp.exe`.

## (Opsional) Langkah 6: Membuat Installer

Jika Anda ingin membuat file installer `.exe` yang dapat didistribusikan:

1.  **Instal Inno Setup**: Unduh dan instal [Inno Setup](https://jrsoftware.org/isinfo.php).
2.  **Buka Script Installer**: Navigasikan ke direktori `live-app\scripts` dan buka file `live-app.iss` dengan Inno Setup Compiler.
3.  **Compile Script**: Di Inno Setup, klik `Build` -> `Compile`.
4.  **Temukan Installer**: File installer (`setup.exe`) akan dibuat di dalam direktori `live-app\scripts\Output`.

Anda sekarang memiliki instalasi `live-app` yang berfungsi penuh di perangkat baru Anda!
