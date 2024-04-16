
#include "items/item.h"
#include "Castle/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/EchoCharacter.h"
#include "NiagaraComponent.h"



Aitem::Aitem()
{
 
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	EmbersEffect->SetupAttachment(GetRootComponent());


}


void Aitem::BeginPlay()
{
	Super::BeginPlay();
	

	UWorld *World = GetWorld();
	//SetActorLocation(FVector(0.f, 0.f, 100.f));
	//SetActorRotation(FRotator(0.f, 180.f, 0.f));
	
	
	//this section of code belong to Template Functions Episode 57
	/*int32 temp = Avg<int32>(5, 3);
	UE_LOG(LogTemp, Warning, TEXT("this is the AVG For this function : %d "), temp );*/
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &Aitem::onSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &Aitem::onSphereEndOverlap);

}

float Aitem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunTime * TimeConstant);
}

float Aitem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunTime * TimeConstant);
}

void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0, 0, TransformedSin()));
	}

	//thats code was used for moving the sphere like circular movment 

	/*DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100);
	DRAW_SPHERE_SingleFrame(GetActorLocation());*/
 
	// and this other used for moving objects by C++ and early in the course we do it with blueprint
	
	/*float UpdatedZ = TransformedSin(RunTime);

	FVector ActorLocationUpdate = Location + FVector(0.f, 0.f, UpdatedZ);
	SetActorLocation(ActorLocationUpdate);*/

	//Episode 57
	/*DRAW_POINT_SingleFrame(Avg<FVector>(GetActorLocation(), FVector::ZeroVector), FColor::Green);*/

	//FRotator UpdateRotator = GetActorRotation() + FRotator(0,100.f*DeltaTime, 0);
	//SetActorRotation(UpdateRotator);

}

void Aitem::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
	
	if (EchoCharacter)
	{
		EchoCharacter->SetOverlappingItem(this);
	}
}

void Aitem::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);

	if (EchoCharacter)
	{
		EchoCharacter->SetOverlappingItem(nullptr);
	}
}

