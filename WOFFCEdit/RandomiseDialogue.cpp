#include "RandomiseDialogue.h"
#include "stdafx.h"
#include "ToolMain.h"


// RandomiseDialogue dialog

IMPLEMENT_DYNAMIC(RandomiseDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(RandomiseDialogue, CDialogEx)
	ON_COMMAND(IDOK, &RandomiseDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &RandomiseDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDRANDOMIZE, &RandomiseDialogue::OnBnClickedRandomize)
	ON_WM_HSCROLL()

END_MESSAGE_MAP()

RandomiseDialogue::RandomiseDialogue(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOGTERRAIN, pParent)
{
}

RandomiseDialogue::~RandomiseDialogue()
{
}

void RandomiseDialogue::SetObjectData(ToolMain* toolMain_) {
	m_ToolMain = toolMain_;


}

void RandomiseDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERAMP, AmpSlider);
	DDX_Control(pDX, IDC_SLIDERFREQ, FreqSlider);
	
}

void RandomiseDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

BOOL RandomiseDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AmpSlider.SetRange(1, 100, true);
	AmpSlider.SetPos(30);
	FreqSlider.SetRange(1, 20, true);
	FreqSlider.SetPos(10);

	//uncomment for modal only
/*	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (size_t i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RandomiseDialogue::PostNcDestroy()
{
}




// RandomiseDialogue message handlers callback   - We only need this if the dailogue is being setup-with createDialogue().  We are doing
//it manually so its better to use the messagemap
/*INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		//	EndDialog(hwndDlg, wParam);
			DestroyWindow(hwndDlg);
			return TRUE;


		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
			break;
		}
	}

	return INT_PTR();
}*/


void RandomiseDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void RandomiseDialogue::OnBnClickedRandomize() {

	m_ToolMain->onActionTerrainRandomize();
}

void RandomiseDialogue::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == TB_THUMBPOSITION) {
		if(*pScrollBar == AmpSlider)
			m_ToolMain->m_d3dRenderer.SetChunkAmp(nPos);
		if (*pScrollBar == FreqSlider)
			m_ToolMain->m_d3dRenderer.SetChunkFreq((nPos / 10.f));

	}
}


