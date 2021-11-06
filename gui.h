#include "pch.h"

struct LaserUnitGui
{	
	int idWindow, idColour, idSlider; float colour[3] = {.0, .0, .0};
	int idDegree[3];
	wxWindow* window;
	wxColourPickerCtrl* picker; // child of window
	wxSlider* slider; // child of window
		
	LaserUnitGui(wxScrolledWindow* guiWindow, wxBoxSizer* windowSizerGui)
	{
		idWindow = wxNewId(); idColour = wxNewId(); idSlider = wxNewId();
		idDegree[0] = wxNewId(); idDegree[1] = wxNewId(); idDegree[2] = wxNewId();
		
		window = new wxWindow(guiWindow, idWindow, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
		picker = new wxColourPickerCtrl(window, idColour);
		slider = new wxSlider(window, idSlider, 255, 0, 255);
		
		wxBoxSizer* windowSizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* color = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* degree = new wxBoxSizer(wxVERTICAL);
		
		wxFlexGridSizer * grid = new wxFlexGridSizer(2, 2, 2);
		
		picker->SetColour(wxColour(colour[0] * 255, colour[1] * 255, colour[2] * 255));
		
		windowSizer->Add(color);
		windowSizer->Add(grid);
		color->Add(picker);
		color->Add(slider);
		grid->Add(new wxStaticText(window, wxID_ANY, "x"));
		grid->Add(new wxTextCtrl(window, idDegree[0], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		grid->Add(new wxStaticText(window, wxID_ANY, "y"));
		grid->Add(new wxTextCtrl(window, idDegree[1], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		grid->Add(new wxStaticText(window, wxID_ANY, "z"));
		grid->Add(new wxTextCtrl(window, idDegree[2], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		
		window->SetSizerAndFit(windowSizer);
		
		guiWindow->SetMinSize(wxSize(window->GetSize().x+25, window->GetSize().y*3));
		guiWindow->SetMaxSize(wxSize(window->GetSize().x+25, window->GetSize().y*3));

		windowSizerGui->Add(window);
		windowSizerGui->AddSpacer(10);
	}
	~LaserUnitGui()
	{
		window->Destroy();
	}
};