namespace GOTHIC_NAMESPACE {
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(
		reinterpret_cast<void*>(zSwitch(0x00731410, 0x007700A0, 0x0077D390, 0x00666610)),
		&oCNpc::OnDamage_Hit_Hook,
		Union::HookType::Hook_Detours
	);

	bool aoeGuard = false;

	bool isDead(oCNpc* npc) {
		if (!npc)
			return false;
		return npc->attribute[NPC_ATR_HITPOINTS] < 1;
	}

	void oCNpc::OnDamage_Hit_Hook(oSDamageDescriptor& desc) {
		(this->*Hook_oCNpc_OnDamage_Hit)(desc);

		oCNpc* attacker = desc.pNpcAttacker;

		if (!attacker || attacker != player)
			return;

		oCItem* weapon = attacker->GetWeapon();
		if (!weapon || (!weapon->IsOneHandWeapon() && !weapon->TwoHanded()))
			return;

		if (aoeGuard)
			return;
		aoeGuard = true;

		float range = weapon->range;

		zCArray<zCVob*> vobsList;
		attacker->CreateVobList(vobsList, range);
		
		oSDamageDescriptor descCopy = oSDamageDescriptor(desc);

		zVEC3 vAttToTarget = (this->GetPositionWorld() - player->GetPositionWorld());

		for (int i = 0; i < vobsList.GetNumInList(); ++i) {
			oCNpc* npc = vobsList[i]->CastTo<oCNpc>();

			if (!npc || npc->IsUnconscious() || isDead(npc) || npc == attacker || npc == this)
				continue;

			zVEC3 vAttToOther = (npc->GetPositionWorld() - player->GetPositionWorld());

			vAttToTarget.n[VY] = 0.0f;
			vAttToOther.n[VY] = 0.0f;
			vAttToTarget.Normalize();
			vAttToOther.Normalize();

			if (vAttToTarget.Dot(vAttToOther) < cosf(60 * PI / 180))
				continue;

			descCopy.pVobHit = npc;
			npc->OnDamage(descCopy);
		}

		aoeGuard = false;
	}
}