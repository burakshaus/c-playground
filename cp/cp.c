#include <fcntl.h>      // open() için gerekli
#include <unistd.h>     // read(), write(), close() için gerekli
#include <stdio.h>      // perror() için
#include <stdlib.h>     // exit() için
#include <errno.h>      // Hata kodları için

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int inputFd, outputFd;    // FILE* yerine tamsayı (int) kullanıyoruz
    ssize_t numRead;          // Okunan byte sayısı (signed size)
    char buffer[BUFFER_SIZE];

    // 1. Argüman Kontrolü
    if (argc != 3) {
        printf("Kullanım: %s <kaynak> <hedef>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 2. Kaynak Dosyayı Aç (Sadece Okuma Modu)
    // O_RDONLY: Read Only
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("Hata: Kaynak dosya açılamadı");
        exit(EXIT_FAILURE);
    }

    // 3. Hedef Dosyayı Aç (Yazma Modu)
    // O_WRONLY: Write Only
    // O_CREAT: Dosya yoksa oluştur
    // O_TRUNC: Dosya varsa içeriğini sil (overwrite)
    // 0666: Dosya izinleri (rw-rw-rw-) - umask ile filtrelenir
    outputFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (outputFd == -1) {
        perror("Hata: Hedef dosya oluşturulamadı");
        close(inputFd);
        exit(EXIT_FAILURE);
    }

    // 4. Döngü: Oku ve Yaz
    // read() fonksiyonu okuduğu byte sayısını döndürür, dosya bitince 0 döner.
    while ((numRead = read(inputFd, buffer, BUFFER_SIZE)) > 0) {
        // write() fonksiyonu yazılan byte sayısını döndürür.
        // Hepsini yazabildik mi diye kontrol ediyoruz.
        if (write(outputFd, buffer, numRead) != numRead) {
            perror("Hata: Yazma işlemi başarısız (Disk dolu olabilir)");
            close(inputFd);
            close(outputFd);
            exit(EXIT_FAILURE);
        }
    }

    // 5. Okuma Hatası Kontrolü
    if (numRead == -1) {
        perror("Hata: Okuma sırasında bir sorun oluştu");
    }

    // 6. Temizlik
    if (close(inputFd) == -1) perror("Hata: Kaynak kapatılamadı");
    if (close(outputFd) == -1) perror("Hata: Hedef kapatılamadı");

    printf("Kopyalama tamamlandı (System Calls ile).\n");
    return 0;
}
