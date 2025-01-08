#include "KinetixLib.h"
#include "ObjectTools.h"
#include "Utils/QrCodeGenerator.h"

FString UKinetixLib::SanitizeObjectName(const FString& InObjectName)
{
	return ObjectTools::SanitizeObjectName(InObjectName);
}

UTexture2D* UKinetixLib::GetQRCode(const FString& QrCodeContent)
{
	Kinetix::qrcodegen::QrCode::Ecc ErrorCorrectionLevel = Kinetix::qrcodegen::QrCode::Ecc::LOW;

	Kinetix::qrcodegen::QrCode Qr =
		Kinetix::qrcodegen::QrCode::encodeText(
			TCHAR_TO_UTF8(*QrCodeContent), ErrorCorrectionLevel);

	uint8 Size = Qr.getSize();
	TArray<FColor> Pixels;
	Pixels.SetNumZeroed(Size * Size);

	FColor Color;
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			Color = Qr.getModule(x, y) ? FColor::White : FColor::Black;
			Pixels[x + y * Size] = Color;
		}
	}

	UTexture2D* QrTexture =
		UTexture2D::CreateTransient(
			Size, Size, PF_B8G8R8A8, TEXT("QrCode"));
	void* Data = QrTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, Pixels.GetData(), Size * Size * 4);
	QrTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	QrTexture->UpdateResource();
	QrTexture->Filter = TF_Nearest;
	
	return QrTexture;
}