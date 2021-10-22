
void ReadBmp(char const *filename, uint32_t *&result, uint16_t &width, uint16_t &height) {
    int i;
    FILE *f;
    auto a=fopen_s(&f, filename, "r");

    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f);

    // extract image height and width from header
    width = *(int *) &info[18];
    height = *(int *) &info[22];

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    auto *data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

    for (i = 0; i < size; i += 3) {
        // flip the order of every 3 bytes
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
        result[i * sizeof(uint32_t)] = data[i];
        result[(i + 1) * sizeof(uint32_t)] = data[i + 1];
        result[(i + 2) * sizeof(uint32_t)] = data[i + 2];
    }
}