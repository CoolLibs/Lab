#define GEN_sample_nearest_neighbour_buffer(buff_name)                     \
    float sample_nearest_neighbour##buff_name(float sampling_point)        \
    {                                                                      \
        int index = int(floor(sampling_point * (buff_name.length() - 1))); \
        if (index < 0 || index >= buff_name.length())                      \
            return 0.;                                                     \
        return buff_name[index];                                           \
    }

#define GEN_sample_linear_buffer(buff_name)                                  \
    float sample_linear##buff_name(float sampling_point)                     \
    {                                                                        \
        float pos    = sampling_point * (buff_name.length() - 1);            \
        int   index1 = int(floor(pos));                                      \
        int   index2 = index1 + 1;                                           \
        if (index2 < 0 || index1 >= buff_name.length())                      \
            return 0.;                                                       \
        float value1 = index1 >= 0 ? buff_name[index1] : 0.;                 \
        float value2 = index2 < buff_name.length() ? buff_name[index2] : 0.; \
        return mix(value1, value2, fract(pos));                              \
    }