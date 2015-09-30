/*------------------------------------------------------------------------------*
 * File Name:				 													*
 * Creation: 																	*
 * Purpose: OriginC Source C file												*
 * Copyright (c) ABCD Corp.	2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010		*
 * All Rights Reserved															*
 * 																				*
 * Modification Log:															*
 *------------------------------------------------------------------------------*/
 
////////////////////////////////////////////////////////////////////////////////////
// Including the system header file Origin.h should be sufficient for most Origin
// applications and is recommended. Origin.h includes many of the most common system
// header files and is automatically pre-compiled when Origin runs the first time.
// Programs including Origin.h subsequently compile much more quickly as long as
// the size and number of other included header files is minimized. All NAG header
// files are now included in Origin.h and no longer need be separately included.
//
// Right-click on the line below and select 'Open "Origin.h"' to open the Origin.h
// system header file.
#include <Origin.h>
#include <..\originlab\fu_utils.h>
#include <..\Originlab\FileImport.h>

////////////////////////////////////////////////////////////////////////////////////

//#pragma labtalk(0) // to disable OC functions for LT calling.

////////////////////////////////////////////////////////////////////////////////////
// Include your own header files here.

#include "EcLabConst.h"
////////////////////////////////////////////////////////////////////////////////////
double masaADividir = 1;

// Start your functions here.

DataRange dataRangeFromCols(string XRange, string YRange){
	DataRange dr;
	dr.Add("X", XRange);
	dr.Add("Y", YRange);
	return dr;
}

DataRange dataRangeFromColPos(Worksheet &wks, int XRangeColPos, int YRangeColPos){
	DataRange dr;
	dr.Add(wks, XRangeColPos , "X");
	dr.Add(wks, YRangeColPos , "Y");
	return dr;
}



void processResistanceData(WorksheetPage &wp){
	out_int("layer number: ", wp.Layers.Count());
	int numLayers = wp.Layers.Count();
		
	for(int i = 0; i < numLayers; i++){
		Worksheet currentWS = wp.Layers(i);
		int newColPos = currentWS.AddCol();
		Column currentImgR(currentWS, RESISTANCE_IMG_POS);
		Column rImgPositive(currentWS, newColPos); 
		Column resitanceReal(currentWS, RESISTANCE_REAL_POS);
		Column freqCol(currentWS, FREC_POS);
		
		
		string formulaLog = " log(col("+ freqCol.GetName() +")) ";
		//freqCol.SetFormula(formulaLog);
		//freqCol.ExecuteFormula();
		
		//copy col
		vector v = currentImgR.GetDataObject();
		vector& vDest = rImgPositive.GetDataObject();
		vDest = v;		
		rImgPositive.SetFormula("-1*col("+rImgPositive.GetName()+")", AU_AUTO);
		rImgPositive.ExecuteFormula();
		
		//Half power col
		newColPos = currentWS.AddCol();
		Column powerHalf(currentWS, newColPos);
		string formula1 = "Power(Power(col("+ rImgPositive.GetName() +"),2) +  Power(col("+ resitanceReal.GetName() +"),2), 0.5)";
		powerHalf.SetFormula(formula1);
		powerHalf.ExecuteFormula();
		
		//bode data
		newColPos = currentWS.AddCol();
		Column frecDat(currentWS, newColPos);
		string formulaFrec = "  Degrees( atan( col("+ rImgPositive.GetName() +")/col("+ resitanceReal.GetName() +")) )";
		frecDat.SetFormula(formulaFrec);
		frecDat.ExecuteFormula();
	}
}

void plotPEISFromWP(WorksheetPage &wspPeis1, WorksheetPage &wspPeis2, bool allCycles){
	string nyquisTemplate = (allCycles) ? TEMPLATE_NA : TEMPLATE_NY;
	string nyquisTheme = THEME_BASE_URL + ((allCycles) ? THEME_NA : THEME_NY );
	string bodeMagnitudTemplate = (allCycles) ? TEMPLATE_BMA : TEMPLATE_BMY ;
	string bodeMagnitudTheme = THEME_BASE_URL + ((allCycles) ? THEME_BMA : THEME_BMY );
	string bodePhaseTemplate = (allCycles) ? TEMPLATE_BPA : TEMPLATE_BPY ;
	string bodePhaseTheme = THEME_BASE_URL + ((allCycles) ? THEME_BPA : THEME_BPY );
	
	GraphPage gpImpedancia;
	gpImpedancia.Create(nyquisTemplate);
	GraphLayer glImpedancia = gpImpedancia.Layers(0);
	gpImpedancia.ApplyFormat(GetAppPath(FALSE) + bodeMagnitudTheme);
	
	GraphPage gpGrad;
	gpGrad.Create(bodePhaseTemplate);
	GraphLayer glGrad = gpGrad.Layers(0);
	gpGrad.ApplyFormat(GetAppPath(FALSE) + bodePhaseTheme );
	
	GraphPage pgLogR;
	pgLogR.Create(bodeMagnitudTemplate);
	GraphLayer glLogR = pgLogR.Layers(0);
	pgLogR.ApplyFormat(GetAppPath(FALSE) + bodeMagnitudTheme );
	
	
	int wspLayerNum = wspPeis1.Layers.Count();
	int j;
	for(j = 0; j < wspLayerNum; j++){
		if( allCycles || (j == 0 || (j+1) == wspLayerNum) ){
			Worksheet currentWS = wspPeis1.Layers(j);
			glImpedancia.AddPlot(dataRangeFromColPos(currentWS, RESISTANCE_REAL_POS, RESISTANCE_IMG_POSITIVO_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			glLogR.AddPlot(dataRangeFromColPos(currentWS, FREC_POS, LOG_RES_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			glGrad.AddPlot(dataRangeFromColPos(currentWS, GRAD_FREQ_POS, FREC_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			
			glImpedancia.UngroupPlots();
			glImpedancia.Rescale();
			
			glLogR.UngroupPlots();
			glLogR.Rescale();
			
			glGrad.UngroupPlots();
			glGrad.Rescale();			
		}				
	}
	
	wspLayerNum = wspPeis2.Layers.Count();
	for(j = 0; j < wspLayerNum; j++){
		if( allCycles || (j == 0 || (j+1) == wspLayerNum) ){
			Worksheet currentWS = wspPeis2.Layers(j);
			glImpedancia.AddPlot(dataRangeFromColPos(currentWS, RESISTANCE_REAL_POS, RESISTANCE_IMG_POSITIVO_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			glLogR.AddPlot(dataRangeFromColPos(currentWS, FREC_POS, LOG_RES_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			glGrad.AddPlot(dataRangeFromColPos(currentWS, GRAD_FREQ_POS, FREC_POS), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			
			glImpedancia.UngroupPlots();
			glImpedancia.Rescale();
			
			glLogR.UngroupPlots();
			glLogR.Rescale();
			
			glGrad.UngroupPlots();
			glGrad.Rescale();			
		}				
	}
	
}

void rutinaArmaPEIS(){
	string strTitle = "Procesamiento de PEIS";
	string strMsg = "Selecciona los archivos .txt de los ciclos correspondientes a la prueba PEIS de carga";
	MessageBox(GetWindow(), strMsg, strTitle, MB_OK);	
	WorksheetPage wspPeis1 = getWSPFromFilecycle();
	
	strMsg = "Selecciona los archivos .txt de los ciclos correspondientes a la prueba PEIS de descarga";
	MessageBox(GetWindow(), strMsg, strTitle, MB_OK);	
	WorksheetPage wspPeis2 = getWSPFromFilecycle();
	
	plotPEISFromWP(wspPeis1, wspPeis2, false);
	plotPEISFromWP(wspPeis1, wspPeis2, true);
}

WorksheetPage getWSPFromFilecycle(){
    int iNumSelFiles;
    WorksheetPage wp;
    StringArray saFilePaths;
    iNumSelFiles = GetMultiOpenBox( saFilePaths, FDLOG_ORIGIN ); // or
    
    if( iNumSelFiles > 0){
    	
		wp.Create( NULL, CREATE_VISIBLE);
    	
    	Worksheet currentWS = wp.Layers();
    	currentWS.Delete(); //Borra el layer que tiene por defecto el nuevo wp
        for( int ii=0; ii<saFilePaths.GetSize(); ii++){
			int nn = wp.AddLayer();
			if(nn >= 0){
				string strFile = saFilePaths[ii]; // some data file name
				ASCIMP	ai;
				if(0 == AscImpReadFileStruct(strFile, &ai) )
				{			 
					// Get active worksheet from active work book.
					Worksheet wks = wp.Layers(nn);				 
					if(0 == wks.ImportASCII(strFile, ai))
						//out_str("Import data successful.");
				}
			}
        }
        processResistanceData(wp);
    }
    else
        printf("You did not select any files!");        
    return wp;
}

void ImportASCII_Ex3(){
	string fileName = GetOpenBox("*.txt");  // txt file browser
	out_str(fileName);
}
int test()          
{
	Worksheet wks = Project.ActiveLayer();	 
	Dataset ds(wks, 1);	 
	for(int ii=0; ii<ds.GetSize(); ii++)
		out_double("", ds[ii]);
	int h = a+b;
	out_int("suma: ", h);
    return 0; // Exit our function, returning zero to the caller
}

void otroTest(){
	Worksheet wks = Project.ActiveLayer();	
	Column colB;
	colB.Attach(wks, 1);
	out_str("col name: " + colB.GetName());
	colB.SetFormula("1+sin(4*col("+colB.GetName()+")*pi/180)", AU_AUTO);
	colB.ExecuteFormula();
}

void lalaTest(){
	double vv = 1.2;
    double v2 = InputBox(vv);
    
    printf("el valor que quieres es: %f", v2);
    if(is_missing_value(v2))
        out_str("User cancelled or invalid");
    else
    {
        
    }
    
    Worksheet wks;
    wks= Project.ActiveLayer();
	

}

void plotPedro(){
	DataRange dr;
	GraphPage gp;
	//gp.Create("Enchulado_1" );
	gp.Create();
	GraphLayer gl = gp.Layers(0);
	int totalBooks = Project.WorksheetPages.Count();
	int currentBookNmb = 0;
	foreach(WorksheetPage wp in Project.WorksheetPages)
    {    	        
		if(currentBookNmb == 0 || (currentBookNmb+1) == totalBooks){
			Worksheet wks = wp.Layers();             
			Column RImg(wks, RESISTANCE_IMG_POS);
			RImg.SetFormula("-1*col("+RImg.GetName()+")", AU_AUTO);
			RImg.ExecuteFormula();
			dr.Add(wks, RESISTANCE_REAL_POS , "X");
			dr.Add(wks, RESISTANCE_IMG_POS , "Y");			
			gl.AddPlot(dr, IDM_PLOT_SCATTER, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
			gl.UngroupPlots();
			gl.Rescale();
			dr.Reset();		
		}
		currentBookNmb++;
        
    }
}

void plotFromWS(Worksheet &currentWS){
	DataRange dr;
	GraphPage gp;
	gp.Create(TEMPLATE_CCA);
	GraphLayer gl = gp.Layers(0);
	if(!gp.ApplyFormat(GetAppPath(FALSE)+ THEME_BASE_URL + THEME_CCA)){
		out_str("No se encontró theme " + GetAppPath(FALSE) + THEME_CCA);
	}
	
	dr.Add(currentWS, CYCLE_NUM_POS , "X");
	dr.Add(currentWS, TCAPACITY_POS , "Y");
	dr.Add(currentWS, CYCLE_NUM_POS + INTEGRATED_CAP_COL_NUM, "X");
	dr.Add(currentWS, TCAPACITY_POS + INTEGRATED_CAP_COL_NUM, "Y");
	
	gl.AddPlot(dr, IDM_PLOT_SCATTER, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
	gl.UngroupPlots();
	dr.Reset();
}

void integrateCyclesFromFiles(){
	WorksheetPage wpC();
	wpC.Create( NULL, CREATE_VISIBLE);    	
	Worksheet newWks = wpC.Layers();  	
    
	int currentWSPage = 0;
	foreach(WorksheetPage wp in Project.WorksheetPages){
		Worksheet wks = wp.Layers();
		int cycleNum;
		cycleNum = wks.GetNumCols()/TOTAL_COL;	
		
		newWks.AddCol();
		newWks.AddCol();
		for(int i = 0; i < cycleNum; i++){    	
			Column cx(wks, (TIME_POS + i*TOTAL_COL));
			Column cy(wks, (I_POS + i*TOTAL_COL));
			IntegrationResult result;
			
			vectorbase &x1 = cx.GetDataObject();
			vectorbase &y1 = cy.GetDataObject();
			
			vector vx = x1;
			vector vy = y1;
			int oErr = ocmath_integrate(vx, vy, 0, (vx.GetSize()-1), &result);
			
			double arCa = abs(result.Area)/(HR_IN_SEC*masaADividir);
			newWks.SetCell(i, 0+currentWSPage*2, i+1);
			newWks.SetCell(i, 1+currentWSPage*2, arCa);
			printf("cycle %d capacity: %f", i+1,arCa);
			printf("\n");
		}
		currentWSPage++;
	}
    
	
	plotFromWS(newWks);
}

void integrationTest(){
	Worksheet wks;
    wks= Project.ActiveLayer();
    int colNum = 5;
    int cycleNum;
    
    cycleNum = wks.GetNumCols()/TOTAL_COL;
    
    Worksheet newWks;
    if( newWks.Create("0", CREATE_VISIBLE_SAME) )
        printf("New worksheet %s with FFT template is created\n", newWks.GetPage().GetName());

	newWks.AddCol();
	newWks.AddCol();

    for(int i = 0; i < cycleNum; i++){
    	
		Column cx(wks, (TIME_POS + i*TOTAL_COL) );
		Column cy(wks, (I_POS + i*TOTAL_COL) );
		IntegrationResult result;
		
		vectorbase &x1 = cx.GetDataObject();
		vectorbase &y1 = cy.GetDataObject();
		
		vector vx = x1;
		vector vy = y1;
		//out_int("tamano vector y: ", vy.GetSize());
		//out_int("tamano vector x: ", vx.GetSize());
		int oErr = ocmath_integrate(vx, vy, 0, (vx.GetSize()-1), &result);
		
		double arCa = abs(result.Area);
		newWks.SetCell(i, 0, i+1);
		newWks.SetCell(i, 1, arCa);
		printf("cycle %d capacity: %f", i+1,arCa);
		printf("\n");
    }
    
	
}

void closeOpenBooks(){
	foreach(WorksheetPage wp in Project.WorksheetPages){
		wp.Destroy();
	}
}

void rutinaEweVsCap(){
	closeOpenBooks();	
	ASCIMP	ai;
	string strTitle = "Procesamiento ciclados";
	string strMsg = "Selecciona el archivo .dat correspondiente a la carga";
	MessageBox(GetWindow(), strMsg, strTitle, MB_OK);
	string 	strFile = GetOpenBox("*.dat");
	
	//carga file
	if(0 == AscImpReadFileStruct(strFile, &ai) ){
		WorksheetPage wp();
		wp.Create( NULL, CREATE_VISIBLE);    	
		Worksheet currentWS = wp.Layers();    
		if(0 == currentWS.ImportASCII(strFile, ai))
			out_str("Import data successful.");
		wp.SetName("Book1-Carga");
	}
	
	//descarga file
	strMsg = "Selecciona el archivo .dat correspondiente a la descarga";
	MessageBox(GetWindow(), strMsg, strTitle, MB_OK);
	strFile = GetOpenBox("*.dat");
    if(0 == AscImpReadFileStruct(strFile, &ai) ){
		WorksheetPage wp();
		wp.Create( NULL, CREATE_VISIBLE);    	
		Worksheet currentWS = wp.Layers();    
		if(0 == currentWS.ImportASCII(strFile, ai))
			out_str("Import data successful.");
		wp.SetName("Book2-Descarga");
	}
	
	//dividir cap entre la masa
    divideByMass();
    
    //graficar primer y ultimo ciclo
    PilotplotEweVsCap(false);
    
    //graficar todos los ciclos
    PilotplotEweVsCap(true);
    
    //Intregrar ciclos 
    integrateCyclesFromFiles();
}

void divideByMass(){
	string strTitle = "Dividir entre masa";
	string strMsg = "¿Desea dividir la columna de capacidad por la masa? \n Asegurate que las unidades de la columna no sean mAh/g";
	int nMB = MB_YESNO|MB_ICONQUESTION;
	if( IDYES == MessageBox(GetWindow(), strMsg, strTitle, nMB) ){
		double massInput = InputBox(1, "Introduce la masa en gramos");
		masaADividir = massInput;
		foreach(WorksheetPage wp in Project.WorksheetPages){
			Worksheet wks = wp.Layers();
			int cycleNum = (wks.GetNumCols()/TOTAL_COL);
			for(int i=0; i < cycleNum; i++){
				Column currentCap(wks, (CAP_POS + i*TOTAL_COL) );
				currentCap.SetFormula("col("+ currentCap.GetName() +")/"+masaADividir+"");
				currentCap.SetUnits("mAh/g");
				currentCap.ExecuteFormula();
			}						
		}
	}
}

void PilotplotEweVsCap(bool allCycles){
	DataRange dr;
	GraphPage gp;
	string EweVsCapTheme = THEME_BASE_URL + ( (allCycles) ? THEME_VCA : THEME_VCY );
	string EweVsCapTemplate = (allCycles) ? TEMPLATE_VCA : TEMPLATE_VCY;
	
	gp.Create( EweVsCapTemplate );	
	GraphLayer gl = gp.Layers(0);
	if(!gp.ApplyFormat(GetAppPath(FALSE)+ EweVsCapTheme)){
		out_str("No se encontró theme " + GetAppPath(FALSE) + EweVsCapTheme);
	}
	
	foreach(WorksheetPage wp in Project.WorksheetPages)
    {    	
        out_str(wp.GetName());
        Worksheet wks = wp.Layers();
        int cycleNum = (wks.GetNumCols()/TOTAL_COL) - 1;
        for(int j = 0; j <= cycleNum; j++){
			if( allCycles || (j == 0 || j == cycleNum) ){
				gl.AddPlot(dataRangeFromColPos(wks, CAP_POS + TOTAL_COL*j, EWE_POS + TOTAL_COL*j), IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
				gl.UngroupPlots();
			}
        }
    }
}

void plotEweVsCap(){
	bool isColNumRight = false;
	int colNum = 4;
	DataRange dr;
	
	GraphPage gp;
	//gp.Create("Enchulado_1" );
	gp.Create();
	GraphLayer gl = gp.Layers(0);
	foreach(WorksheetPage wp in Project.WorksheetPages)
    {
    	
        out_str(wp.GetName());
        Worksheet wks = wp.Layers();
        out_int("Number of columns = ", wks.GetNumCols());
		dr.Add(wks, 0, "X");
		dr.Add(wks, 1, "Y");
		dr.Add(wks, 2, "X");
		dr.Add(wks, 3, "Y");
		gl.AddPlot(dr, IDM_PLOT_LINE, GAP_ALLOW_DUPLICATE_COL | GAP_USE_TEMPLATE);
		gl.UngroupPlots();
		dr.Reset();
    }
}

void plot_xy_from_diff_wks()
{
	Worksheet wks;
    wks= Project.ActiveLayer();
	DataRange dr;
	
	dr.Add(wks, 2, "X");
    dr.Add(wks, 0, "Y");	
    
    dr.Add(wks, 7, "X");
    dr.Add(wks, 5, "Y");
 
	GraphPage gp;
	gp.Create("Enchulado_1", CREATE_HIDDEN);
	GraphLayer gl = gp.Layers(0);
	gl.AddPlot(dr, IDM_PLOT_SCATTER);
	gl.Rescale();
	gp.SetShow();
	out_int("Number of columns to be deleted = ", wks.GetNumCols());
	
	foreach(WorksheetPage wp in Project.WorksheetPages)
    {
        out_str(wp.GetName());
    }

}
void XYRange_GetData_Ex1()
{
    Worksheet wks;
    wks= Project.ActiveLayer();
    XYRange dr;
    if( wks )
    {
        dr.Add(wks, 0, "X");
        dr.Add(wks, 1, "Y");
        dr.Add(wks, 2, "ED");
 
        DWORD dwRules = DRR_BAD_WEIGHT_TREATMENT;
        int nIndex = 0;
        vector vX, vY, vErr;
        if (dr.GetData(vY, vX, &vErr, nIndex, dwRules))
            printf("Data size is %d ", vY.GetSize());
		GraphPage gp;
		gp.Create("origin", CREATE_HIDDEN);
		GraphLayer gl = gp.Layers(0);
		gl.AddPlot(dr, IDM_PLOT_SCATTER);
		gl.Rescale();
		gp.SetShow();
    }    
}
