#pragma once
#include "stdafx.h"

class CMFCOCCView : public CStatic
{
public:
	CMFCOCCView();
	virtual ~CMFCOCCView();
	
protected:
	Handle_Graphic3d_GraphicDriver m_myGraphicDriver;

	Handle(V3d_Viewer) m_myViewer;
	Handle(AIS_InteractiveContext) m_myAISContext;

	Handle_V3d_View m_myView;

	TopoDS_Shape m_Shape;

	Handle(AIS_Shape) m_ais;

	Standard_Integer               m_myXmax;
    Standard_Integer               m_myYmax;

protected:
	virtual void OnPaint();
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()

public:
	void ReadIGESFile(CString strFileName);                       //��ȡigs�ļ�
	void ReadSTEPFile(CString strFileName);
	void SetBackgroundColor(Quantity_NameOfColor color);      //���ñ�����ɫ
	void SetPartColor(Quantity_NameOfColor color);            //���������ɫ
	void SetPartMaterial(Graphic3d_NameOfMaterial material);  //�����������
	void SetPresentMode(Standard_Boolean mode);               //������ʾ�߿���ʵ��
	void ExportBrepFile();
	void SetShape(TopoDS_Shape shape);

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	TopoDS_Shape GetShape() const; 
};