#include "GASDemoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Abilities/GA_Dash.h"
#include "Abilities/GA_Fireball.h"
#include "Abilities/GA_AOESlam.h"
#include "Abilities/GA_Shield.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AGASDemoCharacter::AGASDemoCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // ── GAS ──────────────────────────────────────────────
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(
        TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AGASDemoCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AGASDemoCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // ── GAS ──────────────────────────────────────────────
    if (AbilitySystemComponent)
    {
        AttributeSet = AbilitySystemComponent->GetSet<UGASDemoAttributeSet>();

        if (DefaultAttributes)
        {
            FGameplayEffectContextHandle Context =
                AbilitySystemComponent->MakeEffectContext();
            FGameplayEffectSpecHandle Spec =
                AbilitySystemComponent->MakeOutgoingSpec(
                    DefaultAttributes, 1, Context);
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
                *Spec.Data.Get());
        }

        InitializeAbilities();
    }
}

void AGASDemoCharacter::InitializeAbilities()
{
    if (!HasAuthority()) return;

    for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
    {
        if (Ability)
        {
            AbilitySystemComponent->GiveAbility(
                FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
        }
    }
}

void AGASDemoCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EIC =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(JumpAction, ETriggerEvent::Started, this,
            &ACharacter::Jump);
        EIC->BindAction(JumpAction, ETriggerEvent::Completed, this,
            &ACharacter::StopJumping);
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this,
            &AGASDemoCharacter::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this,
            &AGASDemoCharacter::Look);
        EIC->BindAction(DashAction, ETriggerEvent::Started, this,
            &AGASDemoCharacter::Input_Dash);
        EIC->BindAction(FireballAction, ETriggerEvent::Started, this,
            &AGASDemoCharacter::Input_Fireball);
        EIC->BindAction(AOESlamAction, ETriggerEvent::Started, this,
            &AGASDemoCharacter::Input_AOESlam);
        EIC->BindAction(ShieldAction, ETriggerEvent::Started, this,
            &AGASDemoCharacter::Input_Shield);
    }
}

void AGASDemoCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection =
            FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection =
            FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AGASDemoCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AGASDemoCharacter::Input_Dash()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Input_Dash CALLED ==="));
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("ASC is null"));
        return;
    }
    AbilitySystemComponent->TryActivateAbilityByClass(UGA_Dash::StaticClass());
}

void AGASDemoCharacter::Input_Fireball()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Input_Fireball CALLED ==="));
    if (!AbilitySystemComponent) return;
    AbilitySystemComponent->TryActivateAbilityByClass(UGA_Fireball::StaticClass());
}

void AGASDemoCharacter::Input_AOESlam()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Input_AOESlam CALLED ==="));
    if (!AbilitySystemComponent) return;
    AbilitySystemComponent->TryActivateAbilityByClass(UGA_AOESlam::StaticClass());
}

void AGASDemoCharacter::Input_Shield()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Input_Shield CALLED ==="));
    if (!AbilitySystemComponent) return;
    AbilitySystemComponent->TryActivateAbilityByClass(UGA_Shield::StaticClass());
}