#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

const int a = 100;
const int b = 0x7f;
//constante de tiempo
int HR_IN_SEC = 3600;

//Constantes de ciclados
const int EWE_POS = 0;
const int I_POS = 1;
const int CAP_POS = 2;
const int TIME_POS = 3;

const int TOTAL_COL = 5;

//Constantes de tabla de integracion
const int CYCLE_NUM_POS = 0;
const int TCAPACITY_POS = 1;
const int INTEGRATED_CAP_COL_NUM = 2;

//Constantes PEIS
const int FREC_POS = 0;
const int RESISTANCE_REAL_POS = 1;
const int RESISTANCE_IMG_POS = 2;
const int RESISTANCE_IMG_POSITIVO_POS = 3;
const int LOG_RES_POS = 4;
const int GRAD_FREQ_POS = 5;

//Constantes rutas themes y templates
const string THEME_BASE_URL = "Themes\Graph";

const string THEME_CCA = "\Cap_vs_Ciclo_1_a_10.oth";
const string THEME_NA = "\Nyquist_1_a_10.oth";
const string THEME_NY = "\Nyquist_1_y_10.oth";
const string THEME_VCA = "\Volt_x_Capacidad_1_a_10.oth";
const string THEME_VCY = "\Volt_x_Capacidad_1_y_10.oth";
const string THEME_BMA = "\Bode_Magnitud_1_a_10.oth";
const string THEME_BMY = "\Bode_Magnitud_1_y_10.oth";
const string THEME_BPA = "\Bode_Phase_1_a_10.oth";
const string THEME_BPY = "\Bode_Phase_1_y_10.oth";

const string TEMPLATE_BMA = "Bode_Magnitud_1_a_10";
const string TEMPLATE_BMY = "Bode_Magnitud_1_y_10";
const string TEMPLATE_BPA = "Bode_Phase_1_a_10";
const string TEMPLATE_BPY = "Bode_Phase_1_y_10";
const string TEMPLATE_CCA = "Cap_vs_Ciclo_1_a_10";
const string TEMPLATE_NA = "Nyquist_1_a_10";
const string TEMPLATE_NY = "Nyquist_1_y_10";
const string TEMPLATE_VCA = "Volt_x_Capacidad_1_a_10";
const string TEMPLATE_VCY = "Volt_x_Capacidad_1_y_10";

#endif
