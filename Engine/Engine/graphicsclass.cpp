////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

int TEST;

GraphicsClass::GraphicsClass()
{
	m_Direct2D = 0;
	BuildingsTexture = 0;
	IconTexture = 0;
	BlackBrush = 0;
	Placing = 0;
	TextFormat = 0;
	EffectsTexture = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{

	bool result;
	HRESULT HR;
	ID2D1Bitmap* locText;
	ID2D1RenderTarget* localRenderTarget;
	D2D1_SIZE_F scale;

	// Create the Direct3D object.
	m_Direct2D = new D2DClass;
	if(!m_Direct2D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct2D->Initialize(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct2D.", L"Error", MB_OK);
		return false;
	}
	localRenderTarget = m_Direct2D->GetRenderTarget();
	Map_Size.height = 10 * m_Direct2D->GetResolution().height;
	Map_Size.width = 10 * m_Direct2D->GetResolution().width;
	HR = LoadBitmapFromFile(localRenderTarget, L"Buildings.gif", &locText);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateBitmapBrush(locText, &BuildingsTexture);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}
	locText->Release();

	HR = LoadBitmapFromFile(localRenderTarget, L"Units.gif", &locText);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateBitmapBrush(locText, &UnitTexture);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}
	locText->Release();

	HR = LoadBitmapFromFile(localRenderTarget, L"Icons.gif", &locText);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateBitmapBrush(locText, &IconTexture);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}
	locText->Release();

	HR = LoadBitmapFromFile(localRenderTarget, L"Effects.png", &EffectsTexture);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}

	HR = LoadBitmapFromFile(localRenderTarget, L"Cursors.gif", &locText);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateBitmapBrush(locText, &Cursors);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}
	locText->Release();

	HR = LoadBitmapFromFile(localRenderTarget, L"map1.gif", &locText);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not load textures.", L"Error", MB_OK);
		return false;
	}
	Map_Text_Size = locText->GetPixelSize();

	HR = localRenderTarget->CreateBitmapBrush(locText, &Map);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}
	locText->Release();

	HR = localRenderTarget->CreateSolidColorBrush(ColorF(ColorF::Black), &BlackBrush);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateSolidColorBrush(ColorF(0.0, 1.0, 0.0, 0.5), &GreenBrush);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}

	HR = localRenderTarget->CreateSolidColorBrush(ColorF(1.0, 0.0, 0.0, 0.5), &RedBrush);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create Brush.", L"Error", MB_OK);
		return false;
	}

	IDWriteFactory* m_IDWriteFactory;
	HR = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_IDWriteFactory));
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create WriteFactory.", L"Error", MB_OK);
		return false;
	}
	HR = m_IDWriteFactory->CreateTextFormat(
		L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-us",
		&TextFormat
	);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create TextFormat.", L"Error", MB_OK);
		return false;
	}

	HR = m_Direct2D->GetRenderTarget()->CreateEffect(CLSID_D2D1ColorMatrix, &ShootingEffect);
	if (HR != S_OK)
	{
		MessageBox(hwnd, L"Could not create shoot effect.", L"Error", MB_OK);
		return false;
	}

	ShootingEffect->SetInput(0, EffectsTexture);

	scale.height = Map_Size.height / (float)Map_Text_Size.height;
	scale.width = Map_Size.width / (float)Map_Text_Size.width;
	Map->SetTransform(Matrix3x2F::Scale(scale));

	interfaceRecFirst.bottom = 0.88f * m_Direct2D->GetResolution().height;
	interfaceRecFirst.top = 0.83f * m_Direct2D->GetResolution().height;
	interfaceRecFirst.left = 0.02f * m_Direct2D->GetResolution().height;
	interfaceRecFirst.right = 0.07f * m_Direct2D->GetResolution().height;

	interfaceRecSecond.bottom = 0.88f * m_Direct2D->GetResolution().height;
	interfaceRecSecond.top = 0.83f * m_Direct2D->GetResolution().height;
	interfaceRecSecond.left = 0.09f * m_Direct2D->GetResolution().height;
	interfaceRecSecond.right = 0.14f * m_Direct2D->GetResolution().height;

	interfaceSizeCommand.height = (0.05f * m_Direct2D->GetResolution().height) / 100;
	interfaceSizeCommand.width = (0.05f * m_Direct2D->GetResolution().height) / 100;

	return true;
}


void GraphicsClass::Shutdown()
{
	if(m_Direct2D)
	{
		m_Direct2D->Shutdown();
		delete m_Direct2D;
		m_Direct2D = 0;
	}

	if (BlackBrush)
	{
		BlackBrush->Release();
	}

	if (BuildingsTexture)
	{
		BuildingsTexture->Release();
	}

	if (IconTexture)
	{
		IconTexture->Release();
	}

	if (UnitTexture)
	{
		UnitTexture->Release();
	}
	
	if (EffectsTexture)
	{
		EffectsTexture->Release();
	}

	return;
}


bool GraphicsClass::Frame(GRAPH_INP input)
{
	bool result;

	result = Render(input);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(GRAPH_INP input)
{
	HRESULT HR;
	D2D1_POINT_2F CenSiz;

	CenSiz.x =  (float)(m_Direct2D->GetResolution().width / 2);
	CenSiz.y =  (float)(m_Direct2D->GetResolution().height / 2);

	Matrix3x2F Scale, Translation;
	Scale = Matrix3x2F::Scale(SizeF(input.Scroll / 1000.0f, input.Scroll / 1000.0f),
		CenSiz);
	Translation = Matrix3x2F::Translation(SizeF((float)input.Center.x, (float)input.Center.y));

	m_Direct2D->GetRenderTarget()->BeginDraw();
	m_Direct2D->GetRenderTarget()->SetTransform(Translation * Scale);
	m_Direct2D->GetRenderTarget()->FillRectangle(RectF(0, 0, (float)Map_Size.width, (float)Map_Size.height), Map);

	input.Sparks[1].Render(m_Direct2D->GetRenderTarget(), ShootingEffect);
	input.Sparks[0].Render(m_Direct2D->GetRenderTarget(), ShootingEffect);

	if (Placing != 0)
	{
		DrawObject(Placing);
	}
	TEST = 0;
	for (int i = 0; i < input.AmmountOfObjects; i++)
	{
		if (input.Objects[i] != 0)
		{
			DrawObject(input.Objects[i]);
			/*if (i != 0)
				((UnitClass*)Objects[i])->Draw(m_Direct2D->GetRenderTarget(), GreenBrush);*/
		}
	}
	//QuadTree->Draw(m_Direct2D->GetRenderTarget(), GreenBrush);
	//input.city->Draw_streets(m_Direct2D->GetRenderTarget(), GreenBrush);
	input.boxes->DrawGeom(m_Direct2D->GetRenderTarget(), GreenBrush);
	if (input.path != 0)
		input.path->Draw_streets(m_Direct2D->GetRenderTarget(), GreenBrush);
	m_Direct2D->GetRenderTarget()->SetTransform(Matrix3x2F::Identity());
	DrawInterface();
	DrawCursor();
	DrawTextwnum("FPS: ", input.FPS, Point2F(10, 10), 1);
	HR = m_Direct2D->EndScene();
	if (HR != S_OK)
	{
		return false;
	}
	return true;
}

HRESULT GraphicsClass::LoadBitmapFromFile(ID2D1RenderTarget * pRenderTarget, PCWSTR uri, ID2D1Bitmap ** ppBitmap)
{
	IWICImagingFactory *pIWICFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pIWICFactory
	);

	if (hr != S_OK)
	{
		return hr;
	}

	hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (hr != S_OK)
	{
		return hr;
	}

	hr = pDecoder->GetFrame(0, &pSource);

	if (hr != S_OK)
	{
		return hr;
	}

	hr = pIWICFactory->CreateFormatConverter(&pConverter);

	if (hr != S_OK)
	{
		return hr;
	}

	hr = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	if (hr != S_OK)
	{
		return hr;
	}

	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		ppBitmap
	);

	if (hr != S_OK)
	{
		return hr;
	}


	if (pIWICFactory)
	{
		pIWICFactory->Release();
	}

	if (pDecoder)
	{
		pDecoder->Release();
	}

	if (pSource)
	{
		pSource->Release();
	}

	if (pStream)
	{
		pStream->Release();
	}

	if (pConverter)
	{
		pConverter->Release();
	}

	if (pScaler)
	{
		pScaler->Release();
	}
	return hr;
}

void GraphicsClass::ClearScreen(ID2D1RenderTarget * input)
{
	input->FillRectangle(RectF(0, 0, (float)m_Direct2D->GetResolution().width, (float)m_Direct2D->GetResolution().height), BlackBrush);
	return;
}

bool GraphicsClass::DrawInterface()
{
	D2D1_RECT_F rcTEX;

	if (Mode.JobPrimary == Mode.JOB_SELECTED)
	{
		if (Mode.Unit == OBJTYPES::ENGINEERS)
		{
			rcTEX.bottom = 100;
			rcTEX.left = 0;
			rcTEX.right = 100;
			rcTEX.top = 0;

			IconTexture->SetTransform(Matrix3x2F::Translation(SizeF(interfaceRecFirst.left, interfaceRecFirst.top))
				* Matrix3x2F::Scale(interfaceSizeCommand, Point2F(interfaceRecFirst.left, interfaceRecFirst.top)));
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, BlackBrush);
			m_Direct2D->GetRenderTarget()->DrawRectangle(&interfaceRecFirst, GreenBrush);
			if (Glowed == OBJTYPES::COMMANDCENTER)
			{
				m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, GreenBrush);
			}
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, IconTexture);
		}
		else if (Mode.Unit == OBJTYPES::COMMANDCENTER)
		{
			rcTEX.bottom = 100;
			rcTEX.left = 200;
			rcTEX.right = 300;
			rcTEX.top = 0;

			IconTexture->SetTransform(Matrix3x2F::Translation(SizeF(interfaceRecFirst.left - rcTEX.left, interfaceRecFirst.top - rcTEX.top))
				* Matrix3x2F::Scale(interfaceSizeCommand, Point2F(interfaceRecFirst.left, interfaceRecFirst.top)));
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, BlackBrush);
			m_Direct2D->GetRenderTarget()->DrawRectangle(&interfaceRecFirst, GreenBrush);
			if (Glowed == OBJTYPES::SOLDIER)
			{
				m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, GreenBrush);
			}
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecFirst, IconTexture);

			rcTEX.bottom = 100;
			rcTEX.left = 100;
			rcTEX.right = 200;
			rcTEX.top = 0;

			IconTexture->SetTransform(Matrix3x2F::Translation(SizeF(interfaceRecSecond.left - rcTEX.left, interfaceRecSecond.top - rcTEX.top))
				* Matrix3x2F::Scale(interfaceSizeCommand, Point2F(interfaceRecSecond.left, interfaceRecSecond.top)));
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecSecond, BlackBrush);
			m_Direct2D->GetRenderTarget()->DrawRectangle(&interfaceRecSecond, GreenBrush);
			if (Glowed == OBJTYPES::ENGINEERS)
			{
				m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecSecond, GreenBrush);
			}
			m_Direct2D->GetRenderTarget()->FillRectangle(&interfaceRecSecond, IconTexture);
		}
		else if (Mode.Unit == OBJTYPES::FACTORY)
		{
			char* inputtext = 0;
			char* outputtext = 0;
			const float scale = 0.5;
			switch (((FactoryClass*)Mode.selected)->GetInputProductsinfo())
			{
					case InvObject::Wheat:
					{
						inputtext = "Wheat: ";
						break;
					}

					case InvObject::Flour:
					{
						inputtext = "Flour: ";
						break;
					}

					case InvObject::Iron_ore:
					{
						inputtext = "Iron ore: ";
						break;
					}

					case InvObject::Steel:
					{
						inputtext = "Steel: ";
						break;
					}

					case NULL:
					{
						inputtext = NULL;
						break;
					}

					default:
					{
						inputtext = "input: ";
					}
			}

			switch (((FactoryClass*)Mode.selected)->GetOutputProductsinfo())
			{
				case InvObject::Wheat:
				{
					outputtext = "Wheat: ";
					break;
				}

				case InvObject::Flour:
				{
					outputtext = "Flour: ";
					break;
				}

				case InvObject::Bread:
				{
					outputtext = "Bread: ";
					break;
				}

				case InvObject::Iron_ore:
				{
					outputtext = "Iron ore: ";
					break;
				}

				case InvObject::Steel:
				{
					outputtext = "Steel: ";
					break;
				}

				case InvObject::cutlery:
				{
					outputtext = "Cutlery: ";
					break;
				}

				case NULL:
				{
					outputtext = NULL;
					break;
				}

				default:
				{
					outputtext = "output: ";
				}
			}
			FactoryClass* fac = (FactoryClass*)Mode.selected;
			if (fac->GetAmmountOfMoney() < 0)
				fac->GetAmmountOfMoney();
			if (inputtext)
				DrawTextwnum(inputtext, fac->GetAmmountOfInputProducts(), Point2F(0.02f * m_Direct2D->GetResolution().width, 0.83f * m_Direct2D->GetResolution().height), scale);
			if (outputtext)
				DrawTextwnum(outputtext, fac->GetAmmountOfOutputProducts(), Point2F(0.02f * m_Direct2D->GetResolution().width,(32 * scale) + 0.83f * m_Direct2D->GetResolution().height), scale);
			DrawTextwnum("Money: ", fac->GetAmmountOfMoney(), Point2F(0.02f * m_Direct2D->GetResolution().width, (64 * scale) + 0.83f * m_Direct2D->GetResolution().height), scale);
			DrawTextwnum("Prize: ", (float)fac->GetPriceOfProducts(), Point2F(0.02f * m_Direct2D->GetResolution().width, (96 * scale) + 0.83f * m_Direct2D->GetResolution().height), scale);
			DrawTextwnum("Margin: ", (float)fac->GetMargin() * 100, Point2F(0.02f * m_Direct2D->GetResolution().width, (128 * scale) + 0.83f * m_Direct2D->GetResolution().height), scale);
			DrawTextwnum("Workers: ", fac->GetAmmountOfWorkers(), ((FactoryClass*)Mode.selected)->GetAmmountOfWantedWorkers() ,Point2F(0.1f * m_Direct2D->GetResolution().width, 0.83f * m_Direct2D->GetResolution().height), scale);
			DrawTextwnum("Salary: ", (float)fac->GetSalary() * 30, Point2F(0.1f * m_Direct2D->GetResolution().width, (32 * scale) + 0.83f * m_Direct2D->GetResolution().height), scale);
		}
	}
	return true;
}

void GraphicsClass::DrawCursor()
{
	if (Mode.JOBSecondary == Mode.JOB_MOVING)
	{
		D2D1_RECT_F rc;
		D2D1_SIZE_F rctext;
		double ScaleValue = 0.4;

		rc.left = (float)CursorPos.x;
		rc.top = (float)CursorPos.y;
		rc.right = (float)(CursorPos.x + (100 * ScaleValue));
		rc.bottom = (float)(CursorPos.y + (100 * ScaleValue));
		rctext.height = (float)CursorPos.y;
		rctext.width = (float)(CursorPos.x - 100.0);
		Matrix3x2F Translation, Scale;
		Translation = Matrix3x2F::Translation(rctext);
		Scale = Matrix3x2F::Scale(SizeF((float)ScaleValue, (float)ScaleValue), Point2F((float)CursorPos.x, (float)CursorPos.y));
		Cursors->SetTransform(Translation * Scale);
		m_Direct2D->GetRenderTarget()->FillRectangle(rc, Cursors);
		Cursors->SetTransform(Matrix3x2F::Identity());
		return;
	}
	if (Mode.JobPrimary == Mode.JOB_IDLE || Mode.JobPrimary == Mode.JOB_SELECTED)
	{
		D2D1_RECT_F rc;
		D2D1_SIZE_F rctext;
		double ScaleValue = 0.2;

		rc.left = (float)CursorPos.x;
		rc.top = (float)CursorPos.y;
		rc.right = (float)(CursorPos.x + (100 * ScaleValue));
		rc.bottom = (float)(CursorPos.y + (100 * ScaleValue));
		rctext.height = (float)CursorPos.y;
		rctext.width = (float)CursorPos.x;
		Matrix3x2F Translation, Scale;
		Translation = Matrix3x2F::Translation(rctext);
		Scale = Matrix3x2F::Scale(SizeF((float)ScaleValue, (float)ScaleValue));
		Cursors->SetTransform(Scale * Translation);
		m_Direct2D->GetRenderTarget()->FillRectangle(rc, Cursors);
		Cursors->SetTransform(Matrix3x2F::Identity());
	}
}

void GraphicsClass::DrawObject(Object* input)
{
	Matrix3x2F StartTransform;
	m_Direct2D->GetRenderTarget()->GetTransform(&StartTransform);
	m_Direct2D->GetRenderTarget()->SetTransform(Matrix3x2F::Rotation((float)input->GetRotation(), Point2F(input->GetPosition().x, input->GetPosition().y)) * StartTransform );
	D2D1_RECT_F rc, rchealth, rcmaxhealth;
	rc = RectF(input->GetStartCorner().x, input->GetStartCorner().y,
		input->GetSize().width + input->GetStartCorner().x,
		input->GetSize().height + input->GetStartCorner().y);
	rcmaxhealth = RectF(input->GetStartCorner().x, input->GetStartCorner().y - 50,
		input->GetSize().width + input->GetStartCorner().x,
		input->GetStartCorner().y - 30);

	if (input->GetTextureType() == OBJTYPES::BUILDING)
	{
		BuildingsTexture->SetTransform(Matrix3x2F::Translation(SizeF(input->GetStartCorner().x - input->GetTexture().left, input->GetStartCorner().y - input->GetTexture().top))
			* Matrix3x2F::Scale(SizeF(input->GetScale().width, input->GetScale().height), Point2F(input->GetStartCorner().x, input->GetStartCorner().y)));
		m_Direct2D->GetRenderTarget()->FillRectangle(rc, BuildingsTexture);
		if (input->Selected)
		{
			m_Direct2D->GetRenderTarget()->FillRectangle(rcmaxhealth, BlackBrush);
			rchealth = rcmaxhealth;
			double percentofhealth = input->GetHealth() / input->GetMaxHealth();
			rchealth.right = (float)((input->GetSize().width * percentofhealth) + input->GetStartCorner().x);
			m_Direct2D->GetRenderTarget()->FillRectangle(rchealth, GreenBrush);
		}
		if (((Building*)input)->GetUnitType() == OBJTYPES::FACTORY)
		{
			//m_Direct2D->GetRenderTarget()->FillEllipse(Ellipse(((FactoryClass*)input)->GetEnter(), 10, 10), GreenBrush);
		}
		//BuildingsTexture->SetTransform(Matrix3x2F::Identity());
	}
	else if (input->GetTextureType() == OBJTYPES::UNIT)
	{
		if (((UnitClass*)input)->GetUnitType() == OBJTYPES::EMPLOYEE)
		{
			if (!((EmployeeClass*)input)->isinfactory())
			{
				m_Direct2D->GetRenderTarget()->FillRectangle(rc, GreenBrush);
			}
		}
		else
		{
			UnitTexture->SetTransform(Matrix3x2F::Translation(SizeF(input->GetStartCorner().x - input->GetTexture().left, input->GetStartCorner().y - input->GetTexture().top))
				* Matrix3x2F::Scale(SizeF(input->GetScale().width, input->GetScale().height), Point2F(input->GetStartCorner().x, input->GetStartCorner().y)));
			m_Direct2D->GetRenderTarget()->FillRectangle(rc, UnitTexture);
			//UnitTexture->SetTransform(Matrix3x2F(Matrix3x2F::Identity()));
		}
		if (((UnitClass*)input)->GetUnitType() == OBJTYPES::EMPLOYEE)
		{
			m_Direct2D->GetRenderTarget()->SetTransform(Matrix3x2F::Identity());
			if (((EmployeeClass*)input)->IsAdult())
				m_Direct2D->GetRenderTarget()->DrawRectangle(RectF((float)600, 10 + (10 * TEST), 700, 20 + (10 * TEST)), BlackBrush);
			else
				m_Direct2D->GetRenderTarget()->DrawRectangle(RectF((float)600, 10 + (10 * TEST), 700, 20 + (10 * TEST)), GreenBrush);
			if (((EmployeeClass*)input)->GetHungry() > 0)
				m_Direct2D->GetRenderTarget()->FillRectangle(RectF((float)600, 10 + (10 * TEST), 675 + (0.25 * ((EmployeeClass*)input)->GetHungry()), 20 + (10 * TEST)), GreenBrush);
			else
				m_Direct2D->GetRenderTarget()->FillRectangle(RectF((float)600, 10 + (10 * TEST), 675 + (0.25 * ((EmployeeClass*)input)->GetHungry()), 20 + (10 * TEST)), RedBrush);
			TEST++;
		}
	}
	if (input->Selected == true)
		m_Direct2D->GetRenderTarget()->DrawRectangle(rc, GreenBrush);
	m_Direct2D->GetRenderTarget()->SetTransform(StartTransform);
}

void GraphicsClass::DrawTextwnum(char* c_str, int num, D2D1_POINT_2F pos, float scale)
{
	char tempString[16];
	char fpsString[16];
	// Convert the fps integer to string format.
	_itoa_s(num, tempString, 10);
	// Setup the fps string.
	strcpy_s(fpsString, c_str);
	strcat_s(fpsString, tempString);
	WCHAR output[16];
	short Lenght = 1;
	for (int i = 0; i < 16; i++)
	{
		output[i] = fpsString[i];
		if (output[i] != 0)
		{
			Lenght++;
		}
		else
		{
			break;
		}
	}
	Matrix3x2F native;
	m_Direct2D->GetRenderTarget()->GetTransform(&native);
	m_Direct2D->GetRenderTarget()->SetTransform(Matrix3x2F::Scale(SizeF(scale, scale), pos) * native);
	m_Direct2D->GetRenderTarget()->DrawText(output, Lenght, TextFormat, RectF(pos.x, pos.y, pos.x + 800, pos.y + 600), BlackBrush);
	m_Direct2D->GetRenderTarget()->SetTransform(native);
}

void GraphicsClass::DrawTextwnum(char* c_str, int num1, int num2, D2D1_POINT_2F pos, float scale)
{
	char temp1String[16];
	char temp2String[16];
	char fpsString[16];
	// Convert the fps integer to string format.
	_itoa_s(num1, temp1String, 10);
	_itoa_s(num2, temp2String, 10);
	// Setup the fps string.
	strcpy_s(fpsString, c_str);
	strcat_s(fpsString, temp1String);
	strcat_s(fpsString, "/");
	strcat_s(fpsString, temp2String);
	WCHAR output[64];
	short Lenght = 1;
	for (int i = 0; i < 64; i++)
	{
		output[i] = fpsString[i];
		if (output[i] != 0)
		{
			Lenght++;
		}
		else
		{
			break;
		}
	}
	Matrix3x2F native;
	m_Direct2D->GetRenderTarget()->GetTransform(&native);
	m_Direct2D->GetRenderTarget()->SetTransform(Matrix3x2F::Scale(SizeF(scale, scale), pos) * native);
	m_Direct2D->GetRenderTarget()->DrawText(output, Lenght, TextFormat, RectF(pos.x, pos.y, pos.x + 800, pos.y + 600), BlackBrush);
	m_Direct2D->GetRenderTarget()->SetTransform(native);
}

UINT GraphicsClass::GetGlowed()
{
	return Glowed;
}

D2D1_SIZE_U GraphicsClass::GetMapSize()
{
	return Map_Size;
}

void GraphicsClass::UpdateState(int X, int Y, int Scroll, POINT Center)
{
	CursorPos.x = X;
	CursorPos.y = Y;
	if (Mode.JobPrimary == Mode.JOB_PLACING)
	{
		D2D1_POINT_2F Position;
		double I = (1000.0 / Scroll);
		int J = (float)(X * I - Center.x - (m_Direct2D->GetResolution().width * (I - 1) / 2));
		//J *= I;
		Position.x = (float)J;
		J = (float)(Y * I - Center.y - (m_Direct2D->GetResolution().height * (I - 1) / 2));
		//J *= I;
		Position.y = (float)J;

		Position.x -= (int)Position.x % 20;
		Position.y -= (int)Position.y % 20;
		if (Placing == 0)
		{
			D2D1_RECT_U rc;
			D2D1_SIZE_F siz, scal;
			Placing = new Object;
			if (Mode.Unit == OBJTYPES::COMMANDCENTER)
			{
				rc.bottom = 100;
				rc.left = 0;
				rc.right = 200;
				rc.top = 0;

				siz.height = (float)(0.2 * m_Direct2D->GetResolution().height);
				siz.width = (float)(0.4 * m_Direct2D->GetResolution().height);
				scal.height = (float)((0.2 * m_Direct2D->GetResolution().height) / 100);
				scal.width = (float)((0.4 * m_Direct2D->GetResolution().height) / 200);
			}
			Placing->initialize(0, Position, 0, siz, scal, 100, 100, rc, Point2F(0.5, 0.5), OBJTYPES::BUILDING);
			Glowed = 0;
		}
		else
		{
			Placing->SetPosition(Position);
		}
	}
	else if (Placing != 0)
	{
		delete Placing;
		Placing = 0;
	}
	else if (Mode.JobPrimary == Mode.JOB_SELECTED)
	{
		if (X <= interfaceRecFirst.right && X >= interfaceRecFirst.left && Y >= interfaceRecFirst.top && Y <= interfaceRecFirst.bottom)
		{
			if (Mode.Unit == OBJTYPES::ENGINEERS)
				Glowed = OBJTYPES::COMMANDCENTER;
			if (Mode.Unit == OBJTYPES::COMMANDCENTER)
				Glowed = OBJTYPES::SOLDIER;
		}
		else if (X <= interfaceRecSecond.right && X >= interfaceRecSecond.left && Y >= interfaceRecSecond.top && Y <= interfaceRecSecond.bottom)
		{
			if (Mode.Unit == OBJTYPES::COMMANDCENTER)
				Glowed = OBJTYPES::ENGINEERS;
		}
		else
		{
			Glowed = 0;
		}
	}
	else
	{
		Glowed = 0;
	}

}

