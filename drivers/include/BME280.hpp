#ifndef TEMPLATE_BME_HPP
#define TEMPLATE_BME_HPP

class BME280 {
public:
    BME280();

    void readData();

    int32_t getTemperature();

    uint32_t getPressure();

    uint32_t getHumidity();

protected:
    BME280_InitTypeDef bme280Init;
    BME280_CompParam bme280CompParam;
    bool first_read_flag;
};

#endif //TEMPLATE_BME_HPP
