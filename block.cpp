#include "block.h"

void Block::build(PNG &im, int column, int width) {
	data.resize(width);
	for (int w = 0; w < width; w++) {
		unsigned int cw = column + w;
		vector <HSLAPixel> v(im.height());
		for (unsigned int i = 0; i < im.height(); i++) {
			v[i] = *im.getPixel(cw,i);
		}
		data[w] = v;
	}
}

int Block::width() const {
	return data.size();
}

int Block::height() const {
	return data[0].size();
}

void Block::render(PNG &im, int column) const {
	int w = width();
	int h = height();
	for (int i = 0; i < w; i++) {
		int cw = column + i;
		for (int j = 0; j < h; j++) {
			*im.getPixel(cw, j) = data[i][j];
		}
	}
}

void Block::greyscale() {
	int w = width();
	int h = height();
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			data[i][j].s = 0;
		}
	}
}

