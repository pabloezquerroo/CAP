#include "stencil.h"
#include "immintrin.h"

void ApplyStencil(unsigned char *img_in, unsigned char *img_out, int width, int height) {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j += 32) { // Procesar 32 píxeles a la vez

            // Cargar la vecindad de 3x3 para 32 píxeles
            __m256i val_UL = _mm256_loadu_si256((__m256i*)&img_in[(i-1) * width + j-1]);
            __m256i val_U = _mm256_loadu_si256((__m256i*)&img_in[(i-1) * width + j]);
            __m256i val_UR = _mm256_loadu_si256((__m256i*)&img_in[(i-1) * width + j+1]);
            __m256i val_L = _mm256_loadu_si256((__m256i*)&img_in[(i) * width + j-1]);
            __m256i val_C = _mm256_loadu_si256((__m256i*)&img_in[(i) * width + j]);  // Este es el píxel central
            __m256i val_R = _mm256_loadu_si256((__m256i*)&img_in[(i) * width + j+1]);
            __m256i val_DL = _mm256_loadu_si256((__m256i*)&img_in[(i+1) * width + j-1]);
            __m256i val_D = _mm256_loadu_si256((__m256i*)&img_in[(i+1) * width + j]);
            __m256i val_DR = _mm256_loadu_si256((__m256i*)&img_in[(i+1) * width + j+1]);

            // Inicializa el resultado a 0
            __m256i result = _mm256_setzero_si256();

            // Sumar y restar las vecindades según el stencil
            result = _mm256_subs_epu8(result, val_UL);
            result = _mm256_adds_epu8(result, val_C); // 1 * val_C
            result = _mm256_subs_epu8(result, val_U);
            result = _mm256_adds_epu8(result, val_C); // 2 * val_C
            result = _mm256_subs_epu8(result, val_UR);
            result = _mm256_adds_epu8(result, val_C); // 3 * val_C
            result = _mm256_subs_epu8(result, val_L);
            result = _mm256_adds_epu8(result, val_C); // 4 * val_C
            result = _mm256_subs_epu8(result, val_R);
            result = _mm256_adds_epu8(result, val_C); // 5 * val_C
            result = _mm256_subs_epu8(result, val_DL);
            result = _mm256_adds_epu8(result, val_C); // 6 * val_C
            result = _mm256_subs_epu8(result, val_D);
            result = _mm256_adds_epu8(result, val_C); // 7 * val_C
            result = _mm256_subs_epu8(result, val_DR);

            // Almacenar el resultado en la imagen de salida
            _mm256_storeu_si256((__m256i*)&img_out[i * width + j], result);
        }
    }
}

void CopyImage(unsigned char *img_in, unsigned char *img_out, int width, int height) {

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			img_in[i*width + j] = img_out[i*width + j];
}
