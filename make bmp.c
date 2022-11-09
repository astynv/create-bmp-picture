#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
#pragma warning(disable:4996)

void write_uint32_le(unsigned char* data, int* offset, uint32_t x) {
	// data -- pointer to first element
	data[*offset] = x & 0xff;
	data[*offset + 1] = ((x >> 8) & 0xff);
	data[*offset + 2] = ((x >> 16) & 0xff);
	data[*offset + 3] = ((x >> 24) & 0xff);
	*offset = *offset + 4;
};

void write_uint16_le(unsigned char* data, int* offset, uint16_t x) {
	// data -- pointer to first element
	data[*offset] = x & 0xff;
	data[*offset + 1] = ((x >> 8) & 0xff);
	*offset = *offset + 2;

};

int make_bmp_image(const char* path, int width, int height) {

	int offset = 2;
	int padding = 4 - ((width * 3) % 4);
	if (padding == 4) padding = 0;
	int padded_width = width * 3 + padding;
	int32_t filesize = padded_width * height + 54;

	unsigned char* image = malloc(filesize);
	if (image == NULL)
	{
		return -1;
	}

	FILE* file = fopen(path, "wb");
	if (file == NULL) {
		free(image);
		return -1;
	}


	// Bitmap File Header
	image[0] = 'B';
	image[1] = 'M';
	write_uint32_le(image, &offset, filesize);
	write_uint16_le(image, &offset, 0);
	write_uint16_le(image, &offset, 0);
	write_uint32_le(image, &offset, 14 + 40);
	assert(offset == 14);

	// Bitmap Info Header
	write_uint32_le(image, &offset, 40);
	write_uint32_le(image, &offset, width);
	write_uint32_le(image, &offset, height);
	write_uint16_le(image, &offset, 1);
	write_uint16_le(image, &offset, 24);
	write_uint32_le(image, &offset, 0);
	write_uint32_le(image, &offset, 0);
	write_uint32_le(image, &offset, 0);
	write_uint32_le(image, &offset, 0);
	write_uint32_le(image, &offset, 0);
	write_uint32_le(image, &offset, 0);
	assert(offset == 14 + 40);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			image[offset++] = 0xff;
			image[offset++] = 0x56;
			image[offset++] = 0x00;



		}

		for (int i = 0; i < padding; i++) {
			if (padding == 4) break;
			image[offset] = 0x00;
			offset = offset + 1;
		}
	}

	

	fwrite(image, filesize, 1, file);

	free(image);

	fclose(file);

	return 0;
}

int main() {

	make_bmp_image("path", 15, 15); // 15 x 15 image
		
	return 0;
}