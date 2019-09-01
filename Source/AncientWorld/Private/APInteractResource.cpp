// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/APInteractResource.h"

void AAPInteractResource::GetDamage(float _dmg)
{
	m_CurrentHealth -= _dmg;
	if (m_CurrentHealth <= 0) {
		m_CurrentHealth = 0;

		SpawnItems();
		Destroy();
	}
}

void AAPInteractResource::SpawnItems()
{

}
