#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pico/rand.h>

#define MASTER

uint8_t get_random_byte() {
    return (uint8_t)get_rand_32();
}

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz
                                         //
// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_SDA_PIN = 0;
static const uint I2C_SCL_PIN = 1;

uint8_t status = 0;

// Our handler is called from the I2C ISR, so it must complete quickly. Blocking calls /
// printing to stdio may interfere with interrupt handling.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: // master has written some data
        status = 1;
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data

        // load from memory
        i2c_write_byte_raw(i2c, get_random_byte());
        i2c_write_byte_raw(i2c, status);
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        status = 2;
        break;
    default:
        break;
    }
}

static void setup_slave() {
    gpio_init(I2C_SDA_PIN);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);

    gpio_init(I2C_SCL_PIN);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    // configure I2C0 for slave mode
    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

static void setup_master() {
    gpio_init(I2C_SDA_PIN);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on slave side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(I2C_SDA_PIN);

    gpio_init(I2C_SCL_PIN);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
}

#ifdef MASTER
int main() {
    stdio_init_all();
    setup_master();
    printf("\nI2C slave example\n");

    while(1) {
        printf("querying slave...\n");

        uint8_t buf[32];
        i2c_write_blocking(i2c0, I2C_SLAVE_ADDRESS, buf, 1, true);
        i2c_read_blocking(i2c0, I2C_SLAVE_ADDRESS, buf, 2, false);

        printf("buf[0]: %d, stat: %d\n", buf[0], buf[1]);  // Print as decimal

        // run_master();
        sleep_ms(1000);

    }
}

#else 
int main() {
    stdio_init_all();

    setup_slave();
    while(1) {
        sleep_ms(100);
    }
}

#endif
