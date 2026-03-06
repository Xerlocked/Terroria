// Copyright 2025 @xerlock. All Rights Reserved.


#include "DialogueSystem/PlayerDialogueComponent.h"

#include "DialogueSystem/NPCDialogueComponent.h"
#include "GameFramework/Character.h"

UPlayerDialogueComponent::UPlayerDialogueComponent()
{
}

void UPlayerDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	DialogueManager = GetWorld()->GetSubsystem<UDialogueManagerSubsystem>();

	if (!DialogueManager)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueManager is not available."));
		return;
	}

	DialogueManager->OnDialogueStarted.AddDynamic(this, &UPlayerDialogueComponent::OnDialogueStarted);

	DialogueManager->OnDialogueEnded.AddDynamic(this, &UPlayerDialogueComponent::OnDialogueEnded);

	DialogueManager->OnNodeChanged.AddDynamic(this, &UPlayerDialogueComponent::OnNodeChanged);
}

void UPlayerDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DialogueManager)
	{
		DialogueManager->OnDialogueStarted.RemoveDynamic(this, &UPlayerDialogueComponent::OnDialogueStarted);
		DialogueManager->OnDialogueEnded.RemoveDynamic(this, &UPlayerDialogueComponent::OnDialogueEnded);
		DialogueManager->OnNodeChanged.RemoveDynamic(this, &UPlayerDialogueComponent::OnNodeChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void UPlayerDialogueComponent::TryInteract(ACharacter* TargetNPC)
{
	if (!TargetNPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInteract: TargetNPC 가 nullptr"));
		return;
	}

	if (bIsInDialogue)
	{
		UE_LOG(LogTemp, Log, TEXT("TryInteract: 이미 대화 중"));
		return;
	}

	if (!DialogueManager)
	{
		UE_LOG(LogTemp, Error, TEXT("TryInteract: DialogManager 없음"));
		return;
	}

	ACharacter* Player = GetOwnerAsCharacter();
	if (!Player)
	{
		return;
	}

	UNPCDialogueComponent* NPCDialogComp = TargetNPC->FindComponentByClass<UNPCDialogueComponent>();

	if (!NPCDialogComp)
	{
		UE_LOG(LogTemp, Log, TEXT("%s 는 대화 컴포넌트가 없음"), *TargetNPC->GetName());
		return;
	}

	if (!NPCDialogComp->CanStartDialogue(Player))
	{
		UE_LOG(LogTemp, Log, TEXT("%s 와 대화 불가 (NPC 바쁨 또는 조건 미충족)"),
		       *TargetNPC->GetName());
		return;
	}

	UDialogueData* DialogData = NPCDialogComp->GetActiveDialogue(Player);
	if (!DialogData)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : 유효한 DialogData 없음"), *TargetNPC->GetName());
		return;
	}

	DialogueManager->StartDialogue(Player, TargetNPC, DialogData);
}

void UPlayerDialogueComponent::SelectResponse(int32 ResponseIndex)
{
	if (!bIsInDialogue)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectResponse: 대화 중이 아님"));
		return;
	}

	ACharacter* Player = GetOwnerAsCharacter();
	if (!Player)
	{
		return;
	}

	DialogueManager->SelectResponse(Player, ResponseIndex);
}

void UPlayerDialogueComponent::ExitDialogue()
{
	if (!bIsInDialogue)
	{
		return;
	}

	ACharacter* Player = GetOwnerAsCharacter();
	if (!Player)
	{
		return;
	}

	DialogueManager->EndDialogue(Player);
}

TArray<FDialogueResponse> UPlayerDialogueComponent::GetValidResponses() const
{
	TArray<FDialogueResponse> ValidResponses;

	if (!bIsInDialogue || !DialogueManager)
	{
		return ValidResponses;
	}

	ACharacter* Player = GetOwnerAsCharacter();
	if (!Player)
	{
		return ValidResponses;
	}

	const FDialogueNode* CurrentNode = DialogueManager->GetCurrentNodeInternal(Player);
	if (!CurrentNode)
	{
		return ValidResponses;
	}

	for (const FDialogueResponse& Response : CurrentNode->Responses)
	{
		if (DialogueManager->EvaluateConditions(Player, Response.Conditions))
		{
			ValidResponses.Add(Response);
		}
	}

	return ValidResponses;
}

void UPlayerDialogueComponent::OnDialogueStarted(ACharacter* Player, ACharacter* NPC)
{
	if (Player != GetOwnerAsCharacter())
	{
		return; // 다른 플레이어 이벤트 무시
	}

	bIsInDialogue = true;
	CurrentNPC = NPC;

	OnLocalDialogueStarted.Broadcast(Player, NPC); // UI에게 전달
}

void UPlayerDialogueComponent::OnNodeChanged(ACharacter* Player, const FDialogueNode& Node)
{
	if (Player != GetOwnerAsCharacter())
	{
		return;
	}

	OnLocalNodeChanged.Broadcast(Node); // UI에게 전달
}

void UPlayerDialogueComponent::OnDialogueEnded(ACharacter* Player)
{
	if (Player != GetOwnerAsCharacter())
	{
		return;
	}

	bIsInDialogue = false;
	CurrentNPC = nullptr;

	OnLocalDialogueEnded.Broadcast(); // UI에게 전달
}

ACharacter* UPlayerDialogueComponent::GetOwnerAsCharacter() const
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("UDialogInteractionComponent의 Owner가 ACharacter가 아님"));
	}
	return Player;
}
