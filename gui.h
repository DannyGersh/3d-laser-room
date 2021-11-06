#include "pch.h"

// unit means wan laser, with his colorSizer, degree of rotation, etc ...

struct LaserUnitGui
{	
	int idWindow, idColour, idSlider; int idDegree[3]; float colour[3] = {.0, .0, .0};

	wxWindow* unitWindow;
	
	LaserUnitGui(wxWindow* parent, wxBoxSizer* guiWndSizer, wxBoxSizer* sizerGui=NULL)
	{
		idWindow = wxNewId(); idColour = wxNewId(); idSlider = wxNewId();
		idDegree[0] = wxNewId(); idDegree[1] = wxNewId(); idDegree[2] = wxNewId();
		
		unitWindow = new wxWindow(parent, idWindow, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
		wxColourPickerCtrl* picker = new wxColourPickerCtrl(unitWindow, idColour);
		wxSlider* slider = new wxSlider(unitWindow, idSlider, 255, 0, 255);
		picker->SetColour(wxColour(colour[0] * 255, colour[1] * 255, colour[2] * 255));
		
		wxBoxSizer* unitWindowSizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* colorSizer = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer* degreeGridSizer = new wxFlexGridSizer(2, 2, 2);
		
		unitWindowSizer->Add(colorSizer);
		unitWindowSizer->Add(degreeGridSizer);
		colorSizer->Add(picker);
		colorSizer->Add(slider);
		colorSizer->Add(new wxTextCtrl(unitWindow, wxID_ANY, "r"), 1, wxALL, 5);
		degreeGridSizer->Add(new wxStaticText(unitWindow, wxID_ANY, "x"));
		degreeGridSizer->Add(new wxTextCtrl(unitWindow, idDegree[0], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		degreeGridSizer->Add(new wxStaticText(unitWindow, wxID_ANY, "y"));
		degreeGridSizer->Add(new wxTextCtrl(unitWindow, idDegree[1], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		degreeGridSizer->Add(new wxStaticText(unitWindow, wxID_ANY, "z"));
		degreeGridSizer->Add(new wxTextCtrl(unitWindow, idDegree[2], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		
		unitWindow->SetSizerAndFit(unitWindowSizer);

		if(!sizerGui) { guiWndSizer->Add(unitWindow); guiWndSizer->AddSpacer(10); }
		else { sizerGui->Add(unitWindow, 0, wxALL | wxCENTER, 5); sizerGui->AddSpacer(10); }
	}
	~LaserUnitGui()
	{
		unitWindow->Destroy();
	}
};


struct Gui
{	
	Gui(wxFrame* frame, wxGLCanvas* canvas)
	{
		wxBoxSizer* sizerMain = new wxBoxSizer( wxHORIZONTAL );
		wxWindow* guiWindow = new wxWindow(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
		wxBoxSizer* sizerGui = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizerGui_lasers = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizerGui_master = new wxBoxSizer(wxVERTICAL);
		
		wxScrolledWindow* guiLaserWindow = new wxScrolledWindow(guiWindow, wxNewId(), wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
		
		sizerMain->Add(canvas, 1, wxEXPAND | wxALL, 10 ); 
		sizerMain->Add(guiWindow, 1, wxEXPAND | wxRIGHT | wxUP | wxDOWN | wxBORDER_DOUBLE , 10 );	
		
		sizerGui->Add(guiLaserWindow, 1, wxEXPAND | wxALL, 10);
		sizerGui->Add(new wxButton(guiWindow, wxID_ANY, "Add laser"), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		sizerGui->Add(sizerGui_master, 1, wxEXPAND | wxALL, 10);
		
		guiWindow->SetSizerAndFit(sizerGui);
		guiLaserWindow->SetSizerAndFit(sizerGui_lasers);
		guiLaserWindow->SetScrollRate(5,5);
	
		LaserUnitGui* l1 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
		sizerGui->AddSpacer(20);
		sizerGui->Add(new wxStaticText(guiWindow, wxID_ANY, "Master"), 0, wxEXPAND | wxCENTER, 10);
		LaserUnitGui* l3 = new LaserUnitGui(guiWindow, sizerGui_lasers, sizerGui);
		
		auto size1 = l1->unitWindow->GetSize();
		auto size2 = guiWindow->GetSize();
		guiWindow->SetMinSize(wxSize(size1.x+55, 200));
		guiWindow->SetMaxSize(wxSize(size1.x+55, 1000));
		
		frame->SetSizerAndFit(sizerMain);
		frame->Show();
	}
};