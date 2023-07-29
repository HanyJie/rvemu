#include <stdio.h>
#include<fcntl.h>
int main() {
    int fd = open("run", O_RDONLY);
    printf("%d\n", fd);
    FILE *file = fopen("run", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int data[5];
    size_t elements_read = fread(data, sizeof(int), 5, file);
    if (elements_read != 5) {
        if (feof(file)) {
            printf("Reached end of file\n");
        } else if (ferror(file)) {
            perror("Error reading file");
        }
        fclose(file);
        return 1;
    }

    // 成功读取了5个整数
    for (int i = 0; i < 5; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    fclose(file);
    return 0;
}