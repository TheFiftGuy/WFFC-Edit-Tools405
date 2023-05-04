#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include <vector>

class ToolMain;

class RandomiseDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(RandomiseDialogue)

public:
	RandomiseDialogue(CWnd* pParent = NULL);
	virtual ~RandomiseDialogue();
	void SetObjectData(ToolMain* toolMain_);

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGTERRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	ToolMain* m_ToolMain = nullptr;


	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl AmpSlider;
	CSliderCtrl FreqSlider;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRandomize();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

};


INT_PTR CALLBACK RandoProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);