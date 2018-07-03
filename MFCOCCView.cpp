#include "stdafx.h"
#include "MFCOCCView.h"

CMFCOCCView::CMFCOCCView()
{
	try
    {
       Handle(Aspect_DisplayConnection) aDisplayConnection;
       m_myGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);
    }
    catch(Standard_Failure)
    {
        // AfxMessageBox ("Fatal error during graphic initialization", MB_ICONSTOP);
        ExitProcess (1);
    }
 
	TCollection_ExtendedString a3DName("Visu3D");
	m_myViewer = new V3d_Viewer(m_myGraphicDriver,a3DName.ToExtString()); 
 
	m_myViewer->SetDefaultLights();
	m_myViewer->SetLightOn();
	m_myViewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY);//�ı䱳����ɫ
 
	m_myAISContext =new AIS_InteractiveContext(m_myViewer);  //����һ�������ĵ�
	//m_myAISContext->DefaultDrawer()->UIsoAspect()->SetNumber(11);
	//m_myAISContext->DefaultDrawer()->VIsoAspect()->SetNumber(11);
 
	//��������ʵ�����ʾģʽ
	m_myAISContext->SetDisplayMode(AIS_Shaded,Standard_False);

	
}

CMFCOCCView::~CMFCOCCView()
{
	
}

BEGIN_MESSAGE_MAP(CMFCOCCView, CStatic)
    ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP() 

void CMFCOCCView::OnPaint()
{
	
	CStatic::OnPaint();
	m_myView->Redraw();
}

void CMFCOCCView::PreSubclassWindow()  
{  
    // TODO: �ڴ����ר�ô����/����û���  
    //DWORD dwStyle = GetStyle();  
    //SetWindowLong(GetSafeHwnd(),GWL_STYLE,dwStyle | SS_OWNERDRAW  );  
	m_myView = m_myViewer->CreateView();
	
	m_myView -> SetComputedMode(Standard_False);
	//Handle(Graphic3d_GraphicDriver) aGraphicDriver = ((COCCRobotApp*)AfxGetApp())->GetGraphicDriver();
	Handle(WNT_Window) aWNTWindow = new WNT_Window(this->GetSafeHwnd());  
	m_myView->SetWindow(aWNTWindow); 
 
	if (!aWNTWindow->IsMapped()) aWNTWindow->Map(); 
	aWNTWindow->SetBackground(Quantity_NOC_SLATEBLUE2);

    CStatic::PreSubclassWindow();  
}  

void CMFCOCCView::ReadIGESFile(CString strFileName)
{
	IGESControl_Controller::Init();
	IGESControl_Reader igesReader;

	char cstr[1024] = {0};
    WideCharToMultiByte(CP_UTF8,0,strFileName.GetBuffer(),-1,cstr,1024,0,0);

	igesReader.ReadFile(cstr);
	igesReader.TransferRoots();
	m_Shape = igesReader.OneShape();
	m_myAISContext->RemoveAll();
	m_ais = new AIS_Shape(m_Shape);
    m_myAISContext->SetColor(m_ais,Quantity_NOC_DARKORANGE,Standard_False);
    m_myAISContext->SetMaterial(m_ais,Graphic3d_NOM_GOLD,Standard_False);
    m_myAISContext->Display(m_ais,Standard_True);
}

void CMFCOCCView::ReadSTEPFile(CString strFileName)
{
	STEPControl_Controller::Init();
	STEPControl_Reader stepReader;

	char cstr[1024] = {0};
    WideCharToMultiByte(CP_UTF8,0,strFileName.GetBuffer(),-1,cstr,1024,0,0);

	stepReader.ReadFile(cstr);
	stepReader.TransferRoots();
	m_Shape = stepReader.OneShape();
	m_myAISContext->RemoveAll();
	m_ais = new AIS_Shape(m_Shape);
    m_myAISContext->SetColor(m_ais,Quantity_NOC_DARKORANGE,Standard_False);
    m_myAISContext->SetMaterial(m_ais,Graphic3d_NOM_GOLD,Standard_False);
    m_myAISContext->Display(m_ais,Standard_True);
}

void CMFCOCCView::SetBackgroundColor(Quantity_NameOfColor color)
{
	m_myView->SetBackgroundColor(color);
	m_myView->Redraw();
}

void CMFCOCCView::SetPartColor(Quantity_NameOfColor color)
{
	m_myAISContext->SetColor(m_ais,color,Standard_False);
}

void CMFCOCCView::SetPartMaterial(Graphic3d_NameOfMaterial material)
{
	m_myAISContext->SetMaterial(m_ais,material,Standard_False);
}

void CMFCOCCView::SetPresentMode(Standard_Boolean mode)
{
	m_myView->SetComputedMode(mode);
}

BOOL CMFCOCCView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Standard_Integer factor = 16;

    Standard_Integer xPos = pt.x;
    Standard_Integer yPos = pt.y;

    if (zDelta > 0)
    {
        xPos += factor;
        yPos += factor;
    }
    else
    {
        xPos -= factor;
        yPos -= factor;
    }

    m_myView->Zoom(pt.x, pt.y, xPos, yPos);
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void CMFCOCCView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_myView->StartRotation (point.x, point.y);
	CStatic::OnMButtonDown(nFlags, point);
}




void CMFCOCCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( nFlags & MK_MBUTTON )
	{
		m_myView->Rotation(point.x,point.y);
	}
	else if ( nFlags & MK_LBUTTON )
	{
		m_myView->Pan(point.x-m_myXmax,m_myYmax-point.y); 
		m_myXmax = point.x; 
		m_myYmax = point.y;	
	}
	CStatic::OnMouseMove(nFlags, point);
}


void CMFCOCCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_myXmax = point.x;
	m_myYmax = point.y;
	CStatic::OnLButtonDown(nFlags, point);
}

void CMFCOCCView::ExportBrepFile()
{
	BRepTools::Write(m_Shape,"f:\\brp1.brep");
}

TopoDS_Shape CMFCOCCView::GetShape() const
{
	return m_Shape;
}

void CMFCOCCView::SetShape(TopoDS_Shape shape)
{
	m_Shape = shape;
	m_myAISContext->RemoveAll();
	m_ais = new AIS_Shape(m_Shape);
    m_myAISContext->SetColor(m_ais,Quantity_NOC_DARKORANGE,Standard_False);
    m_myAISContext->SetMaterial(m_ais,Graphic3d_NOM_GOLD,Standard_False);
    m_myAISContext->Display(m_ais,Standard_True);
}