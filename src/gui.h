

struct LaserUnitGui : public wxWindow
{	
	// unit means wan laser, with his colorSizer, degree of rotation, etc ...
	
	int idWindow, idColour, idSlider; int idDegree[3]; float colour[3] = {.0, .0, .0};
	
	LaserUnitGui(wxWindow* parent, wxBoxSizer* guiWndSizer, wxBoxSizer* sizerGui=NULL): wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE)
	{
		idWindow = wxNewId(); idColour = wxNewId(); idSlider = wxNewId();
		idDegree[0] = wxNewId(); idDegree[1] = wxNewId(); idDegree[2] = wxNewId();
		
		wxColourPickerCtrl* picker = new wxColourPickerCtrl(this, idColour);
		wxSlider* slider = new wxSlider(this, idSlider, 255, 0, 255);
		picker->SetColour(wxColour(colour[0] * 255, colour[1] * 255, colour[2] * 255));
		
		wxFlexGridSizer* main = new wxFlexGridSizer(3, 3, 2, 2);
		
		wxBoxSizer* unitWindowSizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* colorSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* reflSizer = new wxBoxSizer(wxHORIZONTAL); // reflections
		
		unitWindowSizer->Add(main);
		main->Add(picker, 1, wxALL | wxEXPAND, 2);
		main->Add(new wxStaticText(this, wxID_ANY, "x"));
		main->Add(new wxTextCtrl(this, idDegree[0], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);

		main->Add(slider, 1, wxALL | wxEXPAND, 2);
		main->Add(new wxStaticText(this, wxID_ANY, "y"));
		main->Add(new wxTextCtrl(this, idDegree[1], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		
		reflSizer->Add(new wxStaticText(this, wxID_ANY, "r"), 0, wxALL, 2);
		reflSizer->Add(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		main->Add(reflSizer);
		
		main->Add(new wxStaticText(this, wxID_ANY, "z"));
		main->Add(new wxTextCtrl(this, idDegree[2], "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
		
		this->SetSizerAndFit(unitWindowSizer);
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
		sizerMain->Add(guiWindow, 1, wxEXPAND | wxRIGHT | wxUP | wxDOWN , 10 );	
		
		sizerGui->Add(guiLaserWindow, 1, wxEXPAND | wxALL, 10);
		sizerGui->Add(new wxButton(guiWindow, wxID_ANY, "Add laser"), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
		sizerGui->Add(sizerGui_master, 1, wxEXPAND | wxALL, 10);
		
		guiWindow->SetSizerAndFit(sizerGui);
		guiLaserWindow->SetSizerAndFit(sizerGui_lasers);
		guiLaserWindow->SetScrollRate(5,5);
		
		LaserUnitGui* l1 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);
		LaserUnitGui* l2 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);			
		sizerGui_lasers->Add(l1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxUP, 10);
		//sizerGui_lasers->AddSpacer(10);
		sizerGui_lasers->Add(l2, 0, wxEXPAND | wxLEFT | wxRIGHT | wxUP, 10);

		sizerGui->AddSpacer(20);
		sizerGui->Add(new wxStaticText(guiWindow, wxID_ANY, "Master"), 0, wxALL | wxCENTER, 2);
		LaserUnitGui* l3 = new LaserUnitGui(guiWindow, sizerGui_lasers, sizerGui);
		sizerGui->Add(l3, 0, wxCENTER, 2);
		sizerGui->AddSpacer(10);
		
		auto size1 = l1->GetSize();
		auto size2 = guiWindow->GetSize();
		guiWindow->SetMinSize(wxSize(size1.x+55, 300));
		guiWindow->SetMaxSize(wxSize(size1.x+55, 1000));
		
		frame->SetSizerAndFit(sizerMain);
		frame->Show();
	}
};