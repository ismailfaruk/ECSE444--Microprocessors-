
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <arm_math.h>	//1
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;
DMA_HandleTypeDef hdma_dac_ch2;	//consider not using

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DAC1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

int voiceSignal[4000] = {508,504,504,504,508,508,512,512,512,516,520,520,520,524,524,524,520,516,512,512,512,512,512,508,508,504,500,496,496,488,472,464,472,468,460,464,472,472,480,496,512,520,532,552,564,568,572,576,572,556,552,544,528,516,508,500,488,488,488,492,492,500,508,512,516,524,528,532,532,536,532,532,532,528,524,524,520,520,516,520,516,516,512,512,504,496,492,480,456,440,440,440,432,436,448,456,468,488,512,528,540,556,568,576,580,584,576,560,548,536,524,512,508,504,496,496,500,508,512,512,516,520,524,532,532,528,524,524,524,520,516,516,516,512,512,520,520,520,520,524,524,520,520,512,500,492,484,460,436,432,436,424,424,436,448,456,476,508,528,540,560,572,576,580,588,580,560,548,544,532,516,512,512,500,496,504,508,508,512,512,512,520,524,524,520,516,520,520,520,524,528,528,532,532,536,536,532,532,532,528,520,512,504,488,476,464,440,416,408,412,408,412,428,444,460,480,512,532,552,568,580,580,580,588,580,568,552,540,532,524,520,520,512,508,512,516,516,516,520,520,520,524,528,524,524,524,524,524,528,532,532,524,524,524,524,520,516,512,504,496,492,484,472,460,448,428,416,424,428,428,440,456,468,484,508,528,544,556,568,572,576,580,584,572,560,548,536,524,520,516,512,504,504,508,512,512,520,524,524,524,532,532,532,532,532,528,528,528,528,524,516,512,512,512,512,508,504,500,496,492,488,484,472,456,436,432,436,436,440,448,456,468,488,512,528,544,556,564,564,568,576,572,560,544,536,524,516,512,512,508,504,508,512,520,528,536,540,536,540,540,540,536,528,520,516,512,512,512,508,504,504,508,512,512,512,512,512,508,500,500,492,476,452,440,440,440,444,448,460,468,476,500,516,532,548,556,556,556,556,556,552,540,528,516,512,512,516,516,520,524,524,532,540,544,540,536,528,520,516,516,512,508,504,508,512,516,524,524,528,528,528,532,532,524,516,508,496,488,480,468,452,436,432,432,436,444,456,464,472,488,504,516,532,540,540,544,544,552,552,552,548,540,536,536,536,536,532,528,524,516,516,520,520,516,512,512,512,520,528,532,536,536,540,544,544,544,540,528,516,512,504,496,492,484,472,468,464,464,464,460,456,452,452,456,464,468,476,476,484,492,504,516,528,536,544,552,560,568,572,568,560,552,540,532,524,520,512,508,504,504,512,516,520,524,528,536,544,548,552,548,540,532,524,520,512,508,500,492,484,484,484,484,488,488,488,488,492,492,492,488,480,476,472,468,468,468,472,476,484,496,508,516,528,540,544,552,556,556,556,552,548,544,536,532,528,528,524,528,528,532,532,532,532,528,528,524,520,516,512,508,504,504,500,496,496,496,496,496,496,496,492,496,496,496,500,500,500,500,500,500,500,500,500,500,500,500,500,504,508,512,512,516,520,524,528,528,528,528,528,524,524,524,524,524,524,524,524,524,524,524,520,520,516,512,512,512,508,508,508,508,508,508,508,508,508,508,504,504,504,504,504,508,508,512,512,512,512,512,512,512,512,512,512,508,508,508,508,512,512,512,516,516,516,516,512,512,508,508,508,508,512,512,512,516,516,520,520,520,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,504,504,504,508,508,512,512,516,516,520,520,520,520,520,516,512,512,512,512,512,512,512,512,512,508,504,504,500,500,500,504,508,512,512,520,520,524,524,520,520,516,512,512,508,508,508,508,512,512,512,512,516,516,512,512,512,508,508,508,508,508,512,512,516,520,524,524,524,524,520,516,512,512,512,508,508,508,508,508,508,508,508,508,508,508,508,508,508,512,512,516,516,520,524,524,520,520,512,512,508,504,504,504,508,512,512,512,516,520,516,516,512,512,508,508,504,504,504,508,512,512,512,516,516,516,516,516,512,512,512,508,508,508,508,508,512,512,512,516,520,520,520,520,516,512,512,512,512,512,512,512,516,512,512,512,512,508,504,504,508,508,512,512,516,516,516,516,516,512,512,512,512,508,508,508,508,508,508,512,512,512,512,512,508,508,504,504,504,504,508,512,512,516,516,520,520,520,520,520,520,516,516,512,512,512,512,512,512,512,512,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,512,512,512,516,516,516,516,516,516,516,512,512,516,516,512,512,512,512,512,512,516,516,516,516,516,516,516,512,512,512,512,512,512,512,508,508,508,508,508,508,508,508,512,512,512,512,512,512,512,508,508,508,508,508,512,512,512,512,512,512,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,512,512,512,512,512,512,512,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,520,520,520,516,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,504,504,504,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,512,512,512,512,512,512,508,508,508,504,504,504,504,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,520,520,520,520,516,516,516,516,512,512,512,512,512,512,508,508,508,508,508,508,508,508,508,508,508,512,512,512,508,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,520,520,520,520,520,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,508,508,508,508,508,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,520,512,512,516,520,512,512,512,512,512,512,512,512,512,512,512,508,512,512,512,508,508,508,508,508,512,512,512,504,508,508,512,508,512,508,512,512,512,508,512,512,512,508,512,512,512,512,512,512,516,516,516,512,520,520,520,516,520,520,516,516,512,516,508,516,508,512,508,512,508,504,512,512,516,512,512,516,512,516,512,516,508,512,508,504,504,504,504,500,508,504,508,508,512,512,516,512,520,512,516,516,516,516,512,512,508,512,512,512,512,508,516,512,516,512,516,520,512,524,512,524,512,516,512,512,512,512,512,512,512,512,512,512,512,512,512,508,512,508,512,504,508,508,508,504,508,512,508,512,508,512,512,512,512,512,516,512,512,508,512,516,512,512,512,512,516,512,516,512,520,512,512,516,512,520,512,512,512,516,512,512,512,512,512,512,512,512,512,512,512,512,508,508,504,504,504,512,508,508,512,512,512,512,512,512,516,512,512,512,512,512,512,508,512,512,512,512,512,516,516,512,512,516,516,512,512,516,516,512,512,512,512,516,512,512,512,516,512,512,512,512,512,512,512,512,512,512,508,512,512,512,512,512,512,508,512,512,508,512,508,512,512,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,512,512,512,516,512,512,512,516,516,516,512,516,520,516,512,512,512,512,512,512,508,512,512,508,512,512,512,508,508,512,512,512,512,512,512,512,512,508,508,512,512,512,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,504,508,508,504,504,504,504,504,504,504,504,504,508,512,512,512,512,516,516,516,520,524,524,524,524,524,524,524,524,524,524,520,520,520,520,516,516,516,516,516,512,512,516,512,512,512,512,512,508,504,500,496,496,492,484,480,480,480,480,476,476,484,492,496,496,508,516,524,528,532,540,544,548,544,544,544,540,532,528,524,520,512,512,512,512,512,508,508,512,516,516,512,520,524,524,520,520,524,520,516,520,520,516,512,512,512,508,504,500,496,488,484,476,460,456,460,456,448,448,464,472,476,488,512,520,532,548,560,568,572,576,572,564,560,552,540,524,520,516,508,504,504,508,504,504,512,516,516,516,520,524,524,524,520,520,524,520,516,516,524,520,516,516,520,516,512,508,500,492,484,464,452,456,452,432,428,448,464,460,468,496,516,524,536,556,572,572,576,576,568,564,556,544,528,524,520,512,504,504,508,508,508,512,512,512,512,516,516,512,512,512,512,512,508,512,512,516,520,524,532,536,536,536,540,532,516,512,508,484,452,444,452,432,408,420,444,448,444,472,508,524,532,552,576,588,588,592,584,576,568,556,536,520,512,508,496,492,496,492,492,496,508,508,508,512,512,512,512,512,512,512,508,512,512,512,516,528,536,532,540,548,548,544,540,536,524,512,504,480,452,448,448,424,404,424,444,440,440,472,508,516,528,552,580,588,592,592,588,584,572,556,540,524,516,504,488,484,488,484,480,488,500,504,508,512,516,520,520,516,520,516,512,512,512,512,508,516,524,524,528,540,548,544,544,544,536,524,516,504,476,460,460,444,412,416,436,436,428,452,488,504,512,536,564,580,588,596,596,592,584,572,552,536,524,508,492,480,480,472,468,476,484,492,496,508,512,520,524,528,532,532,528,524,524,524,516,516,520,524,520,524,528,532,528,528,528,524,512,512,500,480,468,468,452,432,432,444,444,436,456,480,492,504,520,544,560,568,576,584,584,580,572,560,548,536,520,512,504,496,488,484,488,492,496,500,508,512,520,520,528,532,528,524,528,524,520,516,516,520,512,512,520,524,520,520,528,524,516,512,512,504,484,472,472,460,440,436,444,444,436,448,472,484,496,512,536,552,560,572,584,588,584,580,576,564,552,536,524,512,504,496,488,484,484,484,488,492,500,504,508,512,520,520,520,528,528,524,524,524,524,520,520,524,520,516,516,520,516,512,512,508,504,500,488,480,480,476,464,460,464,464,464,468,480,488,496,508,516,528,536,548,556,564,564,564,564,560,552,544,536,528,520,512,512,504,500,496,496,496,496,500,500,504,508,508,512,512,512,516,516,520,520,524,524,524,524,524,520,520,520,512,512,512,508,500,496,492,488,484,480,480,480,480,480,480,484,488,492,500,504,512,512,520,524,528,532,536,540,544,544,544,544,540,536,532,528,524,520,516,512,512,508,508,508,508,504,508,508,512,512,512,512,512,516,516,516,516,512,512,512,512,512,512,512,508,508,504,504,500,500,500,496,496,496,492,492,492,496,496,496,500,504,504,508,512,512,516,520,520,524,524,528,528,528,532,532,532,528,528,528,528,524,524,524,520,520,516,516,516,512,512,512,512,512,508,508,508,508,508,504,504,504,508,508,508,508,508,508,504,504,504,504,504,504,504,500,500,504,504,504,504,504,508,508,512,512,512,512,516,516,520,520,520,524,524,524,524,524,524,524,524,520,520,520,516,516,512,512,512,512,508,508,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,504,504,504,508,508,512,512,512,512,512,512,512,512,516,516,516,516,516,512,512,512,512,512,512,512,512,508,508,508,508,508,508,512,512,512,512,512,512,512,516,516,516,516,516,516,520,516,516,516,516,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,504,504,504,500,500,500,504,504,504,504,508,508,508,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,516,516,516,516,512,512,512,512,512,512,512,516,516,516,516,520,520,520,520,520,516,516,516,512,512,512,508,504,504,500,500,496,496,496,500,500,500,504,504,508,508,512,512,512,512,512,512,516,516,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,516,516,516,516,516,512,512,512,512,512,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,516,516,516,512,512,512,512,512,512,512,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,512,512,512,512,512,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,512,512,512,516,516,516,516,516,520,520,512,512,516,512,512,512,512,512,512,512,512,512,512,512,516,512,516,516,512,512,512,512,512,508,504,508,508,504,508,508,508,512,512,512,512,512,512,520,512,512,516,512,512,512,512,512,512,512,512,512,512,516,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,508,508,508,508,508,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,520,520,520,520,516,516,516,516,512,512,512,508,504,504,500,492,484,488,480,472,472,480,476,480,488,500,504,512,520,532,536,540,552,552,548,552,552,540,532,528,524,512,512,512,508,504,504,508,504,508,512,512,512,512,516,516,512,512,516,516,512,516,516,516,520,524,528,528,528,532,528,524,516,512,508,492,468,472,452,432,432,436,428,432,456,464,480,500,520,536,548,564,584,588,580,588,584,564,556,548,532,516,512,508,500,496,500,504,500,504,512,512,512,520,520,520,516,520,516,512,512,512,512,512,516,520,524,528,532,536,532,532,528,520,512,504,488,456,456,440,412,412,420,412,416,444,456,476,496,520,544,556,572,592,596,588,596,592,572,564,556,540,520,516,512,504,500,504,508,504,508,512,512,512,516,516,512,512,512,508,504,508,512,508,512,516,520,524,532,536,540,540,540,536,524,516,512,496,484,468,444,440,424,408,412,424,420,432,464,472,492,516,536,552,568,584,592,596,592,596,584,564,560,548,528,516,516,508,500,500,504,500,500,508,512,508,512,512,512,512,512,512,512,512,512,512,512,516,524,524,532,536,536,536,536,532,524,520,512,504,492,480,460,432,436,416,400,408,424,416,432,468,476,496,520,544,560,580,596,604,604,600,600,588,568,560,548,528,512,512,500,492,496,496,492,500,504,508,512,512,516,512,516,516,512,512,512,512,512,512,520,520,524,532,536,536,536,536,532,524,520,512,500,484,472,444,428,428,404,400,412,424,420,452,476,484,508,532,552,564,584,596,600,600,596,592,576,564,560,544,528,524,516,508,504,508,504,504,508,512,508,508,512,512,508,512,512,508,508,512,512,512,516,520,524,532,532,532,536,532,524,524,516,508,500,488,472,460,428,432,420,400,412,428,424,436,476,480,496,524,548,556,580,600,600,604,608,604,592,580,572,560,540,528,524,512,500,500,496,488,492,496,492,492,500,504,500,504,508,508,508,512,512,512,516,524,528,532,536,540,536,532,532,528,520,516,508,496,484,476,440,432,428,404,408,424,428,428,464,476,488,516,540,556,576,596,604,608,608,604,596,580,568,556,536,520,516,504,492,492,488,480,484,492,492,492,500,504,504,508,512,512,512,512,516,516,520,524,532,532,536,544,540,540,540,532,524,524,512,500,492,480,448,440,436,408,408,420,424,424,460,468,480,508,532,544,564,588,596,600,600,600,592,576,568,560,536,524,520,508,496,496,492,484,488,492,492,492,500,504,500,508,512,508,512,512,512,512,524,528,532,536,544,544,544,544,544,536,532,528,512,504,492,476,444,444,424,404,412,420,416,428,460,460,484,512,528,544,568,584,592,596,600,600,584,572,568,548,532,524,512,500,496,496,488,484,492,492,492,496,504,504,508,512,512,512,516,520,520,524,532,536,540,548,548,548,552,548,544,540,532,520,512,496,484,456,432,428,400,396,404,404,400,432,444,456,488,512,528,552,580,588,600,608,608,600,588,580,568,544,536,524,512,500,500,492,484,488,492,492,500,504,508,512,512,512,512,516,520,520,524,528,532,540,544,548,552,552,552,548,544,540,528,512,504,488,464,432,428,404,388,396,400,392,412,436,440,472,500,516,540,572,584,596,608,612,608,596,588,576,556,540,532,512,500,500,496,484,488,496,496,504,512,516,516,520,528,528,528,528,528,524,528,528,532,532,532,532,532,532,536,528,528,524,516,504,496,476,444,444,416,400,404,408,396,412,436,432,460,488,508,524,556,572,584,592,600,600,588,580,576,560,544,540,528,512,512,512,508,508,512,512,512,516,520,516,516,520,516,512,512,512,512,516,520,524,528,536,540,544,544,548,544,544,536,524,512,500,472,440,436,404,384,392,392,380,404,424,420,452,480,500,520,556,568,584,600,608,608,604,596,592,572};
uint8_t inputAudio[4000];
uint16_t convertedAudio[4000];
int inputCount = 0;
int timerFlag;
int start = 0;
int reading = 1;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
HAL_StatusTypeDef status;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_DAC1_Init();
  MX_USART1_UART_Init();
	
  /* USER CODE BEGIN 2 */
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);	//hdac1 is the DAC_HandleTypeDef *hdac
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	
	HAL_TIM_Base_Start_IT(&htim2);
	
  //Get 8bits data from the UART transmission, convert it back to 16bits
  

  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//Using GPIO�s in digital mode: Push-button and LED
		if(reading){	//if USER button depressed, return 1
			status = HAL_UART_Receive(&huart1, inputAudio, 4000, 3000);
			if(status == HAL_OK){
				reading = 0;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_Delay(2000);
				int i;
				for(i = 0; i < 4000; i++){
				convertedAudio[i] = inputAudio[i];	//concatenation to get 10 bits data
				}
			}
			else{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			}
		}
		
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
			start = 1;
			
		}
		
		//If button is pressed start audio conversion
		while(start){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			//On timer interrupt convert digital value to analog
			if(timerFlag == 1){
				timerFlag = 0;
				
				if(inputCount == 4000){
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
					HAL_Delay(2000);
					inputCount = 0;
					start = 0;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	//turn off LED
					HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
					HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
					break;
				}
				else{
					//Audio Output On
					HAL_DAC_SetValue (&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 5*convertedAudio[inputCount]);
					HAL_DAC_SetValue (&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 5*convertedAudio[inputCount]);
					inputCount++;
				}
			}
			
		}
  /* USER CODE END WHILE */
	
  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 32;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/* DAC1 init function */
static void MX_DAC1_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

  /**DAC Initialization */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**DAC channel OUT1 config*/
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**DAC channel OUT2 config */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    
    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /*Configure GPIO pin : PB14 */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    
}
static void MX_TIM2_Init(void)
{
    
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 10000; 	//80Mhz 80Mticks/s - 10000ticks -1int = 80M/10000
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
		
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
}
#endif /* USE_FULL_ASSERT */