#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>     // usleep, STDOUT_FILENO
#  include <sys/ioctl.h>  // ioctl, winsize
#endif

const float theta_spacing = 0.05f;
const float phi_spacing   = 0.02f;

const float R1 = 0.5f;    // tube radius
const float R2 = 1.0f;    // central radius
const float K2 = 3.0f;    // distance from viewer to torus (must be > 0)

const float CHAR_ASPECT = 0.5f; // adjust (0.45..0.7) for your terminal/font

const char *LUMINANCE_CHARS = ".,-~:;=!*#$@";

static void enable_ansi_on_windows(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

static void get_terminal_size(int *cols, int *rows) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hOut, &csbi)) {
        *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return;
    }
    *cols = 80; *rows = 24;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0 && w.ws_row > 0) {
        *cols = (int)w.ws_col;
        *rows = (int)w.ws_row;
    } else {
        *cols = 80;
        *rows = 24;
    }
#endif
}

static void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep((useconds_t)ms * 1000);
#endif
}

static void render_frame(float A, float B, int screen_width, int screen_height,
                         char *output, float *zbuffer)
{
    const float K1 = (float)screen_width * K2 * 3.0f / (8.0f * (R1 + R2));
    float cosA = cosf(A), sinA = sinf(A);
    float cosB = cosf(B), sinB = sinf(B);

    size_t total = (size_t)screen_width * (size_t)screen_height;
    memset(output, ' ', total);
    for (size_t i = 0; i < total; ++i) zbuffer[i] = 0.0f;

    for (float theta = 0.0f; theta < 2.0f * M_PI; theta += theta_spacing) {
        float costheta = cosf(theta), sintheta = sinf(theta);
        for (float phi = 0.0f; phi < 2.0f * M_PI; phi += phi_spacing) {
            float cosphi = cosf(phi), sinphi = sinf(phi);
            float circlex = R2 + R1 * costheta;
            float circley = R1 * sintheta;

            float x = circlex * (cosB * cosphi + sinA * sinB * sinphi)
                    - circley * cosA * sinB;
            float y = circlex * (sinB * cosphi - sinA * cosB * sinphi)
                    + circley * cosA * cosB;
            float z = K2 + cosA * circlex * sinphi + circley * sinA;
            float ooz = 1.0f / z;

            int xp = (int)((screen_width  / 2.0f) + K1 * ooz * x + 0.5f);
            int yp = (int)((screen_height / 2.0f) - K1 * ooz * y * CHAR_ASPECT + 0.5f);

            float L = cosphi * costheta * sinB
                    - cosA * costheta * sinphi
                    - sinA * sintheta
                    + cosB * (cosA * sintheta - costheta * sinA * sinphi);

            if (L > 0.0f && xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
                size_t idx = (size_t)yp * (size_t)screen_width + (size_t)xp;
                if (ooz > zbuffer[idx]) {
                    zbuffer[idx] = ooz;
                    int luminance_index = (int)(L * 8.0f);
                    int max_index = (int)strlen(LUMINANCE_CHARS) - 1;
                    if (luminance_index < 0) luminance_index = 0;
                    if (luminance_index > max_index) luminance_index = max_index;
                    output[idx] = LUMINANCE_CHARS[luminance_index];
                }
            }
        }
    }

    printf("\x1b[H");
    for (int y = 0; y < screen_height; ++y) {
        fwrite(output + (size_t)y * screen_width, 1, screen_width, stdout);
        putchar('\n');
    }
    fflush(stdout);
}

int main(void) {
    enable_ansi_on_windows();

    int screen_width, screen_height;
    get_terminal_size(&screen_width, &screen_height);
    if (screen_height < 4) screen_height = 4;

    size_t total = (size_t)screen_width * (size_t)screen_height;
    char *output = malloc(total);
    if (!output) { fprintf(stderr, "Failed to allocate output\n"); return 1; }
    float *zbuffer = malloc(total * sizeof(float));
    if (!zbuffer) { fprintf(stderr, "Failed to allocate zbuffer\n"); free(output); return 1; }

    printf("\x1b[2J");

    float A = 0.0f, B = 0.0f;
    const int frame_sleep_ms = 30;

    while (1) {
        int new_w, new_h;
        get_terminal_size(&new_w, &new_h);
        if (new_w != screen_width || new_h != screen_height) {
            screen_width = new_w;
            screen_height = new_h;
            total = (size_t)screen_width * (size_t)screen_height;
            char *noutput = realloc(output, total);
            float *nz = realloc(zbuffer, total * sizeof(float));
            if (noutput) output = noutput;
            if (nz) zbuffer = nz;
            printf("\x1b[2J");
        }

        render_frame(A, B, screen_width, screen_height, output, zbuffer);
        A += 0.07f;
        B += 0.03f;
        sleep_ms(frame_sleep_ms);
    }

    free(output);
    free(zbuffer);
    return 0;
}