/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include "ov2640.h"
#include "dvp.h"
#include "plic.h"
#include "board.h"

#if defined SVGA_25FPS_MODE
const uint8_t ov2640_config[][2]=
{
    {0xff, 0x01},
    {0x12, 0x80},
    {0xff, 0x00},
    {0x2c, 0xff},
    {0x2e, 0xdf},
    {0xff, 0x01},
    {0x3c, 0x32},
    {0x11, 0x00},
    {0x09, 0x02},
    {0x04, 0x28},
    {0x13, 0xe5},
    {0x14, 0x48},
    {0x2c, 0x0c},
    {0x33, 0x78},
    {0x3a, 0x33},
    {0x3b, 0xfb},
    {0x3e, 0x00},
    {0x43, 0x11},
    {0x16, 0x10},
    {0x39, 0x92},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {0x34, 0xc0},
    {0x36, 0x1a},
    {0x06, 0x88},
    {0x07, 0xc0},
    {0x0d, 0x87},
    {0x0e, 0x41},
    {0x4c, 0x00},
    {0x48, 0x00},
    {0x5b, 0x00},
    {0x42, 0x03},
    {0x4a, 0x81},
    {0x21, 0x99},
    {0x24, 0x40},
    {0x25, 0x38},
    {0x26, 0x82},
    {0x5c, 0x00},
    {0x63, 0x00},
    {0x46, 0x22},
    {0x0c, 0x3c},
    {0x61, 0x70},
    {0x62, 0x80},
    {0x7c, 0x05},
    {0x20, 0x80},
    {0x28, 0x30},
    {0x6c, 0x00},
    {0x6d, 0x80},
    {0x6e, 0x00},
    {0x70, 0x02},
    {0x71, 0x94},
    {0x73, 0xc1},
    {0x3d, 0x34},
    {0x5a, 0x57},
    {0x12, 0x40},
    {0x17, 0x11},
    {0x18, 0x43},
    {0x19, 0x00},
    {0x1a, 0x97},
    {0x32, 0x09},
    {0x37, 0xc0},
    {0x4f, 0xca},
    {0x50, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x3d, 0x38},
    {0xff, 0x00},
    {0xe5, 0x7f},
    {0xf9, 0xc0},
    {0x41, 0x24},
    {0xe0, 0x14},
    {0x76, 0xff},
    {0x33, 0xa0},
    {0x42, 0x20},
    {0x43, 0x18},
    {0x4c, 0x00},
    {0x87, 0xd5},
    {0x88, 0x3f},
    {0xd7, 0x03},
    {0xd9, 0x10},
    {0xd3, 0x82},
    {0xc8, 0x08},
    {0xc9, 0x80},
    {0x7c, 0x00},
    {0x7d, 0x00},
    {0x7c, 0x03},
    {0x7d, 0x48},
    {0x7d, 0x48},
    {0x7c, 0x08},
    {0x7d, 0x20},
    {0x7d, 0x10},
    {0x7d, 0x0e},
    {0x90, 0x00},
    {0x91, 0x0e},
    {0x91, 0x1a},
    {0x91, 0x31},
    {0x91, 0x5a},
    {0x91, 0x69},
    {0x91, 0x75},
    {0x91, 0x7e},
    {0x91, 0x88},
    {0x91, 0x8f},
    {0x91, 0x96},
    {0x91, 0xa3},
    {0x91, 0xaf},
    {0x91, 0xc4},
    {0x91, 0xd7},
    {0x91, 0xe8},
    {0x91, 0x20},
    {0x92, 0x00},
    {0x93, 0x06},
    {0x93, 0xe3},
    {0x93, 0x05},
    {0x93, 0x05},
    {0x93, 0x00},
    {0x93, 0x04},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x96, 0x00},
    {0x97, 0x08},
    {0x97, 0x19},
    {0x97, 0x02},
    {0x97, 0x0c},
    {0x97, 0x24},
    {0x97, 0x30},
    {0x97, 0x28},
    {0x97, 0x26},
    {0x97, 0x02},
    {0x97, 0x98},
    {0x97, 0x80},
    {0x97, 0x00},
    {0x97, 0x00},
    {0xc3, 0xed},
    {0xa4, 0x00},
    {0xa8, 0x00},
    {0xc5, 0x11},
    {0xc6, 0x51},
    {0xbf, 0x80},
    {0xc7, 0x10},
    {0xb6, 0x66},
    {0xb8, 0xa5},
    {0xb7, 0x64},
    {0xb9, 0x7c},
    {0xb3, 0xaf},
    {0xb4, 0x97},
    {0xb5, 0xff},
    {0xb0, 0xc5},
    {0xb1, 0x94},
    {0xb2, 0x0f},
    {0xc4, 0x5c},
    {0xc0, 0x64},
    {0xc1, 0x4b},
    {0x8c, 0x00},
    {0x86, 0x3d},
    {0x50, 0x00},
    {0x51, 0xc8},
    {0x52, 0x96},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x5a, 0xc8},
    {0x5b, 0x96},
    {0x5c, 0x00},
    {0xd3, 0x02},
    {0xc3, 0xed},
    {0x7f, 0x00},
    {0xda, 0x08},
    {0xe5, 0x1f},
    {0xe1, 0x67},
    {0xe0, 0x00},
    {0xdd, 0x7f},
    {0x05, 0x00},
    {0xff, 0x00},
    {0xe0, 0x04},
    {0x5a, 0x50},
    {0x5b, 0x3c},
    {0x5c, 0x00},
    {0xe0, 0x00},
    {0x00, 0x00}
};
#elif defined UXGA_15FPS_MODE
const uint8_t ov2640_config[][2]=
{
    {0xFF, 0x00},
    {0x2C, 0xFF},
    {0x2E, 0xDF},
    {0xFF, 0x01},
    {0x3C, 0x32},
    {0x11, 0x00},
    {0x09, 0x02},
    {0x04, 0x28},
    {0x13, 0xE5},
    {0x14, 0x48},
    {0x2C, 0x0C},
    {0x33, 0x78},
    {0x3A, 0x33},
    {0x3B, 0xFB},
    {0x3E, 0x00},
    {0x43, 0x11},
    {0x16, 0x10},
    {0x39, 0x92},
    {0x35, 0xDA},
    {0x22, 0x1A},
    {0x37, 0xC3},
    {0x23, 0x00},
    {0x34, 0xC0},
    {0x36, 0x1A},
    {0x06, 0x88},
    {0x07, 0xC0},
    {0x0D, 0x87},
    {0x0E, 0x41},
    {0x4C, 0x00},
    {0x48, 0x00},
    {0x5B, 0x00},
    {0x42, 0x03},
    {0x4A, 0x81},
    {0x21, 0x99},
    {0x24, 0x40},
    {0x25, 0x38},
    {0x26, 0x82},
    {0x5C, 0x00},
    {0x63, 0x00},
    {0x46, 0x00},
    {0x0C, 0x3C},
    {0x61, 0x70},
    {0x62, 0x80},
    {0x7C, 0x05},
    {0x20, 0x80},
    {0x28, 0x30},
    {0x6C, 0x00},
    {0x6D, 0x80},
    {0x6E, 0x00},
    {0x70, 0x02},
    {0x71, 0x94},
    {0x73, 0xC1},
    {0x3D, 0x34},
    {0x5A, 0x57},
    {0x12, 0x00},
    {0x17, 0x11},
    {0x18, 0x75},
    {0x19, 0x01},
    {0x1A, 0x97},
    {0x32, 0x36},
    {0x03, 0x0F},
    {0x37, 0x40},
    {0x4F, 0xCA},
    {0x50, 0xA8},
    {0x5A, 0x23},
    {0x6D, 0x00},
    {0x6D, 0x38},
    {0xFF, 0x00},
    {0xE5, 0x7F},
    {0xF9, 0xC0},
    {0x41, 0x24},
    {0xE0, 0x14},
    {0x76, 0xFF},
    {0x33, 0xA0},
    {0x42, 0x20},
    {0x43, 0x18},
    {0x4C, 0x00},
    {0x87, 0xD5},
    {0x88, 0x3F},
    {0xD7, 0x03},
    {0xD9, 0x10},
    {0xD3, 0x82},
    {0xC8, 0x08},
    {0xC9, 0x80},
    {0x7C, 0x00},
    {0x7D, 0x00},
    {0x7C, 0x03},
    {0x7D, 0x48},
    {0x7D, 0x48},
    {0x7C, 0x08},
    {0x7D, 0x20},
    {0x7D, 0x10},
    {0x7D, 0x0E},
    {0x90, 0x00},
    {0x91, 0x0E},
    {0x91, 0x1A},
    {0x91, 0x31},
    {0x91, 0x5A},
    {0x91, 0x69},
    {0x91, 0x75},
    {0x91, 0x7E},
    {0x91, 0x88},
    {0x91, 0x8F},
    {0x91, 0x96},
    {0x91, 0xA3},
    {0x91, 0xAF},
    {0x91, 0xC4},
    {0x91, 0xD7},
    {0x91, 0xE8},
    {0x91, 0x20},
    {0x92, 0x00},
    {0x93, 0x06},
    {0x93, 0xE3},
    {0x93, 0x05},
    {0x93, 0x05},
    {0x93, 0x00},
    {0x93, 0x04},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x96, 0x00},
    {0x97, 0x08},
    {0x97, 0x19},
    {0x97, 0x02},
    {0x97, 0x0C},
    {0x97, 0x24},
    {0x97, 0x30},
    {0x97, 0x28},
    {0x97, 0x26},
    {0x97, 0x02},
    {0x97, 0x98},
    {0x97, 0x80},
    {0x97, 0x00},
    {0x97, 0x00},
    {0xC3, 0xEF},
    {0xA4, 0x00},
    {0xA8, 0x00},
    {0xC5, 0x11},
    {0xC6, 0x51},
    {0xBF, 0x80},
    {0xC7, 0x10},
    {0xB6, 0x66},
    {0xB8, 0xA5},
    {0xB7, 0x64},
    {0xB9, 0x7C},
    {0xB3, 0xAF},
    {0xB4, 0x97},
    {0xB5, 0xFF},
    {0xB0, 0xC5},
    {0xB1, 0x94},
    {0xB2, 0x0F},
    {0xC4, 0x5C},
    {0xC0, 0xC8},
    {0xC1, 0x96},
    {0x8C, 0x00},
    {0x86, 0x3D},
    {0x50, 0x00},
    {0x51, 0x90},
    {0x52, 0x2C},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x88},
    {0x5A, 0x90},
    {0x5B, 0x2C},
    {0x5C, 0x05},
    {0xD3, 0x02},
    {0xC3, 0xED},
    {0x7F, 0x00},
    {0xDA, 0x08},
    {0xE5, 0x1F},
    {0xE1, 0x67},
    {0xE0, 0x00},
    {0xDD, 0x7F},
    {0x05, 0x00},
    {0xe0, 0x04},
    {0x5a, 0x50},
    {0x5b, 0x3c},
    {0x5c, 0x00},
    {0xe0, 0x00},
    {0x00, 0x00}
};
#else
const uint8_t ov2640_config[][2]=
{
    {0x00,0x00}
};
#endif



int ov2640_init(void)
{
    uint16_t v_manuf_id;
    uint16_t v_device_id;
    ov2640_read_id(&v_manuf_id, &v_device_id);
    printf("manuf_id:0x%04x,device_id:0x%04x\n", v_manuf_id, v_device_id);
    uint16_t index = 0;
    for (index = 0; ov2640_config[index][0]; index++)
        dvp_sccb_send_data(OV2640_ADDR, ov2640_config[index][0], ov2640_config[index][1]);
    return 0;
}

int ov2640_read_id(uint16_t *manuf_id, uint16_t *device_id)
{
    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x01);
    *manuf_id = (dvp_sccb_receive_data(OV2640_ADDR, 0x1C) << 8) | dvp_sccb_receive_data(OV2640_ADDR, 0x1D);
    *device_id = (dvp_sccb_receive_data(OV2640_ADDR, 0x0A) << 8) | dvp_sccb_receive_data(OV2640_ADDR, 0x0B);
    return 0;
}

int SensorConfigure(struct CameraCfg *cfg_info)
{
    uint8_t reg_tmp;

    //set reg mode to sensor
    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x00);

    //configure the window size and position
    dvp_sccb_send_data(OV2640_ADDR, 0x51, cfg_info->window_w>>2);
    dvp_sccb_send_data(OV2640_ADDR, 0x52, cfg_info->window_h>>2);
    dvp_sccb_send_data(OV2640_ADDR, 0x53, (uint8_t)(cfg_info->window_xoffset&0xFF));
    dvp_sccb_send_data(OV2640_ADDR, 0x54, (uint8_t)(cfg_info->window_yoffset&0xFF));
    dvp_sccb_send_data(OV2640_ADDR, 0x55, (((cfg_info->window_h/4)&0x100)>>1)+
                                          ((cfg_info->window_yoffset&0x700)>>4)+
                                          (((cfg_info->window_w/4)&0x100)>>5)+
                                          ((cfg_info->window_xoffset&0x700)>>8));
    dvp_sccb_send_data(OV2640_ADDR, 0x57, ((cfg_info->window_w/4)&0x200)>>2);
    dvp_sccb_send_data(OV2640_ADDR, 0x5A, cfg_info->output_w>>2);   
    dvp_sccb_send_data(OV2640_ADDR, 0x5B, cfg_info->output_h>>2);  

    //set reg mode to dsp
    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x01);
    
    //configure dsp gain
    if(cfg_info->gain_manu_enable){
        reg_tmp = dvp_sccb_receive_data(OV2640_ADDR, 0x13);
        dvp_sccb_send_data(OV2640_ADDR, 0x13, reg_tmp&0xFB);  
        dvp_sccb_send_data(OV2640_ADDR, 0x00, cfg_info->gain);    
    }else{
        reg_tmp = dvp_sccb_receive_data(OV2640_ADDR, 0x13);
        dvp_sccb_send_data(OV2640_ADDR, 0x13, reg_tmp|0x04);          
    }
    

    return 1;
}

const uint8_t OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
    {
        0xFF,0x01,
        0x24,0x20,
        0x25,0x18,
        0x26,0x60,
    },
    {
        0xFF,0x01,
        0x24,0x34,
        0x25,0x1c,
        0x26,0x00,
    },
    {
        0xFF,0x01,
        0x24,0x3e,
        0x25,0x38,
        0x26,0x81,
    },
    {
        0xFF,0x01,
        0x24,0x48,
        0x25,0x40,
        0x26,0x81,
    },
    {
        0xFF,0x01,
        0x24,0x58,
        0x25,0x50,
        0x26,0x92,
    },
};

const uint8_t ov2640_yuv422_reg_tbl[][2] =
{
    {0xFF, 0x00},
    {0xDA, 0x10},
    {0xD7, 0x03},
    {0xDF, 0x00},
    {0x33, 0x80},
    {0x3C, 0x40},
    {0xe1, 0x77},
    {0x00, 0x00},
};

const uint8_t ov2640_jpeg_reg_tbl[][2]=
{
    {0xff, 0x01},
    {0xe0, 0x14},
    {0xe1, 0x77},
    {0xe5, 0x1f},
    {0xd7, 0x03},
    {0xda, 0x10},
    {0xe0, 0x00},
};

const uint8_t ov2640_rgb565_reg_tbl[][2]=
{
    {0xFF, 0x00},
    {0xDA, 0x08},
    {0xD7, 0x03},
    {0xDF, 0x02},
    {0x33, 0xa0},
    {0x3C, 0x00},
    {0xe1, 0x67},

    {0xff, 0x01},
    {0xe0, 0x00},
    {0xe1, 0x00},
    {0xe5, 0x00},
    {0xd7, 0x00},
    {0xda, 0x00},
    {0xe0, 0x00},
};


/* change ov2640 to jpeg mode */
void ov2640_jpeg_mode(void)
{
    uint16_t i=0;
    /* set yun422 mode */
    for (i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++)
    {
        dvp_sccb_send_data(OV2640_ADDR, ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]);
    }

    /* set jpeg mode */
    for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
    {
        dvp_sccb_send_data(OV2640_ADDR, ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);
    }
}

/* change ov2640 to rgb565 mode */
void ov2640_rgb565_mode(void)
{
    uint16_t i=0;
    for (i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 2); i++)
    {
        dvp_sccb_send_data(OV2640_ADDR, ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]);
    }
}



/* set auto exposure level value must be 0 ~4 */
void ov2640_set_auto_exposure(uint8_t level)
{
    uint8_t i = 0;
    uint8_t *p = (uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
    for (i = 0; i < 4; i++)
    {
        dvp_sccb_send_data(OV2640_ADDR, p[i*2],p[i*2+1]);
    }
}

/* set light mode
 * 0: auto
 * 1: sunny
 * 2: cloudy
 * 3: office
 * 4: home
 * */
void ov2640_set_light_mode(uint8_t mode)
{
    uint8_t regccval, regcdval, regceval;

    switch(mode)
    {
        case 0:
            dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x00);
            dvp_sccb_send_data(OV2640_ADDR, 0xC7, 0x10);
            return;

        case 2:
            regccval = 0x65;
            regcdval = 0x41;
            regceval = 0x4F;
            break;

        case 3:
            regccval = 0x52;
            regcdval = 0x41;
            regceval = 0x66;
            break;

        case 4:
            regccval = 0x42;
            regcdval = 0x3F;
            regceval = 0x71;
            break;

        default:
            regccval = 0x5E;
            regcdval = 0x41;
            regceval = 0x54;
            break;
    }

    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0xC7, 0x40);
    dvp_sccb_send_data(OV2640_ADDR, 0xCC, regccval);
    dvp_sccb_send_data(OV2640_ADDR, 0xCD, regcdval);
    dvp_sccb_send_data(OV2640_ADDR, 0xCE, regceval);
}

/* set color saturation
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: +1
 * 4: +2
 * */
void ov2640_set_color_saturation(uint8_t sat)
{
    uint8_t reg7dval = ((sat+2)<<4) | 0x08;
    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0X00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7C, 0X00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7D, 0X02);
    dvp_sccb_send_data(OV2640_ADDR, 0x7C, 0X03);
    dvp_sccb_send_data(OV2640_ADDR, 0x7D, reg7dval);
    dvp_sccb_send_data(OV2640_ADDR, 0x7D, reg7dval);
}

/* set brightness
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
void ov2640_set_brightness(uint8_t bright)
{
  dvp_sccb_send_data(OV2640_ADDR, 0xff, 0x00);
  dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x00);
  dvp_sccb_send_data(OV2640_ADDR, 0x7d, 0x04);
  dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x09);
  dvp_sccb_send_data(OV2640_ADDR, 0x7d, bright << 4);
  dvp_sccb_send_data(OV2640_ADDR, 0x7d, 0x00);
}

/* set contrast
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
void ov2640_set_contrast(uint8_t contrast)
{
    uint8_t reg7d0val, reg7d1val;

    switch(contrast)
    {
        case 0:
            reg7d0val = 0x18;
            reg7d1val = 0x34;
            break;

        case 1:
            reg7d0val = 0x1C;
            reg7d1val = 0x2A;
            break;

        case 3:
            reg7d0val = 0x24;
            reg7d1val = 0x16;
            break;

        case 4:
            reg7d0val = 0x28;
            reg7d1val = 0x0C;
            break;

        default:
            reg7d0val = 0x20;
            reg7d1val = 0x20;
            break;
    }
    dvp_sccb_send_data(OV2640_ADDR, 0xff, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, 0x04);
    dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x07);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, 0x20);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, reg7d0val);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, reg7d1val);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, 0x06);
}

/* set special effects
 * 0: noraml
 * 1: negative film
 * 2: black-and-white
 * 3: the red
 * 4: the green
 * 5: the blue
 * 6: Retro
*/
void ov2640_set_special_effects(uint8_t eft)
{
    uint8_t reg7d0val, reg7d1val, reg7d2val;

    switch(eft)
    {
        case 1:
            reg7d0val = 0x40;
            break;
        case 2:
            reg7d0val = 0x18;
            break;
        case 3:
            reg7d0val = 0x18;
            reg7d1val = 0x40;
            reg7d2val = 0xC0;
            break;
        case 4:
            reg7d0val = 0x18;
            reg7d1val = 0x40;
            reg7d2val = 0x40;
            break;
        case 5:
            reg7d0val = 0x18;
            reg7d1val = 0xA0;
            reg7d2val = 0x40;
            break;
        case 6:
            reg7d0val = 0x18;
            reg7d1val = 0x40;
            reg7d2val = 0xA6;
            break;
        default:
            reg7d0val = 0x00;
            reg7d1val = 0x80;
            reg7d2val = 0x80;
            break;
    }
    dvp_sccb_send_data(OV2640_ADDR, 0xff, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, reg7d0val);
    dvp_sccb_send_data(OV2640_ADDR, 0x7c, 0x05);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, reg7d1val);
    dvp_sccb_send_data(OV2640_ADDR, 0x7d, reg7d2val);
}

/* set the image output window */
void ov2640_set_window_size(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
    uint16_t endx;
    uint16_t endy;
    uint8_t temp;
    endx = sx + width / 2;
    endy = sy + height / 2;

    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x01);
    temp = dvp_sccb_receive_data(OV2640_ADDR,0x03);
    temp &= 0xF0;
    temp |= ((endy & 0x03) << 2) | (sy & 0x03);
    dvp_sccb_send_data(OV2640_ADDR, 0x03, temp);
    dvp_sccb_send_data(OV2640_ADDR, 0x19, sy>>2);
    dvp_sccb_send_data(OV2640_ADDR, 0x1A, endy>>2);

    temp = dvp_sccb_receive_data(OV2640_ADDR,0x32);
    temp &= 0xC0;
    temp |= ((endx & 0x07) << 3) | (sx & 0x07);
    dvp_sccb_send_data(OV2640_ADDR, 0x32, temp);
    dvp_sccb_send_data(OV2640_ADDR, 0x17, sx>>3);
    dvp_sccb_send_data(OV2640_ADDR, 0x18, endx>>3);
}

/* set the image output size */
uint8_t ov2640_set_image_out_size(uint16_t width,uint16_t height)
{
    uint16_t outh, outw;
    uint8_t temp;

    if(width%4)return 1;
    if(height%4)return 2;
    outw = width /4;
    outh = height/4;
    dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x00);
    dvp_sccb_send_data(OV2640_ADDR, 0xE0, 0x04);
    dvp_sccb_send_data(OV2640_ADDR, 0x5A, outw & 0XFF);
    dvp_sccb_send_data(OV2640_ADDR, 0x5B, outh & 0XFF);
    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    dvp_sccb_send_data(OV2640_ADDR, 0x5C, temp);
    dvp_sccb_send_data(OV2640_ADDR, 0xE0, 0X00);

    return 1;
}

/* set the image window size */
uint8_t ov2640_set_image_window_size(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t hsize, vsize;
    uint8_t temp;
    if ((width % 4) || (height%4))
    {
        return EOF;
    }
    hsize = width /4;
    vsize = height/4;
   dvp_sccb_send_data(OV2640_ADDR, 0XFF,0X00);
   dvp_sccb_send_data(OV2640_ADDR, 0XE0,0X04);
   dvp_sccb_send_data(OV2640_ADDR, 0X51,hsize&0XFF);
   dvp_sccb_send_data(OV2640_ADDR, 0X52,vsize&0XFF);
   dvp_sccb_send_data(OV2640_ADDR, 0X53,offx&0XFF);
   dvp_sccb_send_data(OV2640_ADDR, 0X54,offy&0XFF);
   temp=(vsize>>1)&0X80;
   temp|=(offy>>4)&0X70;
   temp|=(hsize>>5)&0X08;
   temp|=(offx>>8)&0X07;
   dvp_sccb_send_data(OV2640_ADDR, 0X55,temp);             //
   dvp_sccb_send_data(OV2640_ADDR, 0X57,(hsize>>2)&0X80);  //
   dvp_sccb_send_data(OV2640_ADDR, 0XE0,0X00);
   return 0;
}

/* set output resolution */
uint8_t ov2640_set_image_size(uint16_t width ,uint16_t height)
{
   uint8_t temp;
   dvp_sccb_send_data(OV2640_ADDR, 0xFF, 0x00);
   dvp_sccb_send_data(OV2640_ADDR, 0xE0, 0x04);
   dvp_sccb_send_data(OV2640_ADDR, 0xC0, (width >>3) & 0xFF);
   dvp_sccb_send_data(OV2640_ADDR, 0xC1, (height >> 3) & 0xFF);
   temp = (width & 0x07) << 3;
   temp |= height & 0x07;
   temp |= (width >> 4) & 0x80;
   dvp_sccb_send_data(OV2640_ADDR, 0x8C, temp);
   dvp_sccb_send_data(OV2640_ADDR, 0xE0, 0x00);

   return 1;
}
