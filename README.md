# Tugas Magang ITS Robocon - Pola Turtlesim

Repositori ini berisi program ROS 2 (C++) untuk menggerakkan kura-kura di Turtlesim secara otomatis. 
Program ini dibuat untuk menggambar pola bunga atau pola batik dengan rapi.

## Fitur Program
- **Bergerak Otomatis:** Kura-kura bisa menggambar pola bingkai luar dan lingkaran dalam secara berurutan dari awal sampai akhir.
- **Pindah Tempat Tanpa Coretan:** Kura-kura otomatis mengangkat pena saat berpindah posisi, jadi tidak ada garis lurus berantakan yang tertinggal di layar.
- **Pola Rapi:** Program sengaja melompati beberapa bagian lingkaran agar gambarnya tidak saling tumpang tindih.

## Cara Menyiapkan (Build) Program
1. Buka terminal dan masuk ke folder workspace ROS 2 kamu:
   ```Bash:
   cd ~/ros2_ws
   colcon build --packages-select tugasmagang2_turtlesim
   source install/setup.bash
   ```
2. Jalankan 2 terminal secara bersamaan:
   ```Terminal 1:
   ros2 run turtlesim turtlesim_node
   ```
   ```Terminal 2:
   ros2 run tugasmagang2_turtlesim pola_node
   ```

##Video Penjelasan
[Link Gdrive](https://drive.google.com/file/d/1v5vEjFOpfK6otUyj9PJxbicU83rhy6zC/view?usp=drive_link)
