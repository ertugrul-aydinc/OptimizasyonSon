// Odev.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <time.h>

int faktoriyelHesapla(int);

int main()
{
    clock_t baslangic, bitis;
    float gecen_zaman;
    int degiskenSayisi;
    int kisitAdedi;
    int choice;

base1:
    printf("Kac adet degisken olacak ?\n");
    scanf_s("%d", &degiskenSayisi);
    if (!(degiskenSayisi == 1 || degiskenSayisi == 2)) {
        printf("Degisken sayisi 1 veya 2 olabilir. Tekrar giriniz\n");
        goto base1;
    }

    float amacMatrix[degiskenSayisi];

    for (int i = 0; i < degiskenSayisi; i++) {
        printf("X%d in katsayisini giriniz: ", i + 1);
        scanf_s("%f", &amacMatrix[i]);
    }

base2:
    printf("Minimize(0) or Maximize(1) ?\n");
    scanf_s("%d", &choice);
    if (!(choice == 0 || choice == 1)) {
        printf("Gecerli bir secim giriniz\n");
        goto base2;
    }

base3:
    printf("Kac adet kisit fonksiyonunuz olacak ?\n");
    scanf_s("%d", &kisitAdedi);
    if (kisitAdedi < 1) {
        printf("En az 1 kisit fonksiyonunuz olmalidir.\n");
        goto base3;
    }

    float kisitMatrix[6][2 + 1];

    for (int i = 0; i < kisitAdedi; i++) {
        for (int j = 0; j < degiskenSayisi + 1; j++) {
            if (j != degiskenSayisi) {
                printf("%d. kisit fonksiyonununuzun X%d katsayisini giriniz: ", i + 1, j + 1);
                scanf_s("%f", &kisitMatrix[i][j]);
            }
            else {
                printf("%d. kisit fonksiyonunuzun esitlik degerini giriniz: ", i + 1);
                scanf_s("%f", &kisitMatrix[i][j]);
            }
        }
    }

    int isaretler[kisitAdedi];

    for (int i = 0; i < kisitAdedi; i++) {
        base4:
        printf("%d. denklemin esitlik durumunu giriniz: <=(1) , >=(0): ", i + 1);
        scanf_s("%d", &isaretler[i]);
        if (!(isaretler[i] == 0 || isaretler[i] == 1)) {
            printf("Hatali giris\n");
            goto base4;
        }

    }

    baslangic = clock();
    //Kombinasyon hesaplandi.
    int hesapAdedi = faktoriyelHesapla(kisitAdedi) / (faktoriyelHesapla(degiskenSayisi) * faktoriyelHesapla(kisitAdedi - degiskenSayisi));
    //
    float tmpKisitMatrix[degiskenSayisi+ 1];
    float cozumMatrix[hesapAdedi][degiskenSayisi];

    int x = 0;
    //Tüm noktalar bulundu
    for (int i = 0; i < kisitAdedi; i++) {
        for (int j = i + 1; j < kisitAdedi; j++) {
            for (int k = 0; k < degiskenSayisi + 1; k++) {
                if(kisitMatrix[i][0] != 0)
                    tmpKisitMatrix[k] = (kisitMatrix[i][k] * kisitMatrix[j][0]) / -(kisitMatrix[i][0]);
                else {
                    if(k<degiskenSayisi)
                        tmpKisitMatrix[k+1] = (kisitMatrix[i][k+1] * kisitMatrix[j][1]) / -(kisitMatrix[i][1]);
                }
            }
            if (kisitMatrix[i][0] != 0) {
                cozumMatrix[x][1] = (tmpKisitMatrix[degiskenSayisi] + kisitMatrix[j][degiskenSayisi]) / (tmpKisitMatrix[1] + kisitMatrix[j][1]);
                cozumMatrix[x][0] = (kisitMatrix[i][degiskenSayisi] - (cozumMatrix[x][1] * kisitMatrix[i][1])) / kisitMatrix[i][0];
                x++;
            }
            else {
                if (!(kisitMatrix[i][0] == 0 && kisitMatrix[j][1] == 0)) {
                    cozumMatrix[x][0] = (tmpKisitMatrix[degiskenSayisi] + kisitMatrix[j][degiskenSayisi]) / kisitMatrix[j][0];
                    cozumMatrix[x][1] = (kisitMatrix[j][degiskenSayisi] - (cozumMatrix[x][0] * kisitMatrix[j][0])) / kisitMatrix[j][1];
                    x++;
                }
                else{
                    cozumMatrix[x][1] = kisitMatrix[i][degiskenSayisi] / kisitMatrix[i][1];
                    cozumMatrix[x][0] = kisitMatrix[j][degiskenSayisi] / kisitMatrix[j][0];
                    x++;
                }
            }

        }
    }

    float possibleCevapMatrix[hesapAdedi][degiskenSayisi];

    int mumkunMu = 0;

    //Mümkün noktalar bulundu
    for (int i = 0; i < hesapAdedi; i++) {
        int flag = 1;
        for (int k = 0; k < kisitAdedi; k++) {
            if (isaretler[k]) {
                if (!((cozumMatrix[i][0] * kisitMatrix[k][0] + cozumMatrix[i][1] * kisitMatrix[k][1]) <= kisitMatrix[k][degiskenSayisi])) {
                    flag = 0;
                    mumkunMu++;
                    break;
                }
            }
            else {
                if (!((cozumMatrix[i][0] * kisitMatrix[k][0] + cozumMatrix[i][1] * kisitMatrix[k][1]) >= kisitMatrix[k][degiskenSayisi])) {
                    flag = 0;
                    mumkunMu++;
                    break;
                }
            }
        }
        if (flag) {
            possibleCevapMatrix[i-mumkunMu][0] = cozumMatrix[i][0];
            possibleCevapMatrix[i-mumkunMu][1] = cozumMatrix[i][1];
        }

    }

    float zDegerleri[hesapAdedi];


    for (int i = 0; i < hesapAdedi - mumkunMu; i++) {
        for (int j = 0; j < degiskenSayisi; j++) {
            zDegerleri[i] += possibleCevapMatrix[i][j] * amacMatrix[j];
        }
    }

    int nokta = 0;
    float sonuc = zDegerleri[0];

    if (mumkunMu == hesapAdedi) {
        printf("Cozumsuz problem");
    }
    else {
        if (choice) {
            for (int i = 1; i < hesapAdedi - mumkunMu; i++) {
                if (zDegerleri[i] > sonuc) {
                    sonuc = zDegerleri[i];
                    nokta = i;
                }
            }
            printf("Max deger: %f\n", sonuc);
            printf("Degerler:\n X1 : %f , X2 : %f\n", possibleCevapMatrix[nokta][0], possibleCevapMatrix[nokta][1]);
        }
        else {
            for (int i = 1; i < hesapAdedi - mumkunMu; i++) {
                if (zDegerleri[i] < sonuc) {
                    sonuc = zDegerleri[i];
                    nokta = i;
                }
            }
            printf("Min deger: %f\n", sonuc);
            printf("Degerler =>  X1 : %f , X2 : %f\n", possibleCevapMatrix[nokta][0], possibleCevapMatrix[nokta][1]);
        }
    }
    bitis = clock();
    gecen_zaman = ((float)(bitis - baslangic)) / CLOCKS_PER_SEC;
    printf("Gecen sure: %f\n", gecen_zaman);

    return 0;
}

int faktoriyelHesapla(int sayi) {
    if (sayi == 1 || sayi == 0) {
        return 1;
    }

    return sayi * faktoriyelHesapla(sayi - 1);
}
