#include "madoka.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define ENTITY_HP 100
#define SHOUJO_KIMOJI 100
#define MAHOUSHOUJO_ATK 100
#define MAHOUSHOUJO_HP (ENTITY_HP * 3)
#define MAJO_ATK 10
#define MAJO_HP (ENTITY_HP * 50)

// #region Entity
Entity* Entity_ctor(Entity* this) {
    this->hp = ENTITY_HP;
    this->is_dead = &Entity_is_dead;
    return this;
}

void Entity_dtor(Entity* this) {
    return;
}

int Entity_is_dead(void* this) {
    return ((Entity*)this)->hp <= 0;
}
// #endregion

// #region Shoujo
Shoujo* Shoujo_ctor(Shoujo* this, const char* name, const char* wish) {
    Entity_ctor((Entity*)this);
    this->name = (char*)malloc(strlen(name) + 1);
    strcpy(this->name, name);
    this->wish = (char*)malloc(strlen(wish) + 1);
    strcpy(this->wish, wish);
    this->kimoji = SHOUJO_KIMOJI;
    this->is_dead = &Entity_is_dead;
    this->is_despair = &Shoujo_is_despair;
    this->do_wish = &Shoujo_do_wish;
    this->despair = &Shoujo_despair;
    return this;
}

void Shoujo_dtor(Shoujo* this) {
    // if (this->name != NULL) {
    //     free(this->name);
    // }
    // if (this->wish != NULL) {
    //     free(this->wish);
    // }
    Entity_dtor((Entity*)this);
}

int Shoujo_is_despair(void* this) {
    return ((Shoujo*)this)->kimoji <= -100;
}

void Shoujo_do_wish(void* this) {
    puts(((Shoujo*)this)->wish);
}

void Shoujo_despair(void* this) {
    ((Entity*)this)->hp = 0;
}
// #endregion

// #region Mahoushoujo
Mahoushoujo* Mahoushoujo_ctor(Mahoushoujo* this, const char* name, const char* wish, Skill skill) {
    Shoujo_ctor((Shoujo*)this, name, wish);
    this->atk = MAHOUSHOUJO_ATK;
    ((Entity*)this)->hp = MAHOUSHOUJO_HP;
    ((Shoujo*)this)->despair = &Mahoushoujo_despair;
    this->is_dead = &Entity_is_dead;
    this->do_wish = &Mahoushoujo_do_wish;
    this->attack = &Mahoushoujo_attack;
    this->skill = skill;
    return this;
}

void Mahoushoujo_dtor(Mahoushoujo* this) {
    Shoujo_dtor((Shoujo*)this);
}

void Mahoushoujo_do_wish(void* this) {
    ((Shoujo*)this)->do_wish((Shoujo*)this);
    puts(" But nothing is good");
    ((Shoujo*)this)->kimoji -= 10;
}

void Mahoushoujo_attack(Mahoushoujo* this, Entity* enemy) {
    ((Entity*)enemy)->hp -= this->atk;
}

void Mahoushoujo_despair(void* this) {
    Shoujo_despair(this);
    puts("Watashii de, hondo baga");
}
// #endregion

// #region Majo
Majo* Majo_ctor(Majo* this, const char* name, const char* wish) {
    Mahoushoujo_ctor((Mahoushoujo*)this, name, wish, NULL);
    this->atk = MAJO_ATK;
    ((Entity*)this)->hp = MAJO_HP;
    this->is_dead = &Entity_is_dead;
    this->attack = &Majo_attack;
    this->kekkai = &Majo_kekkai;
    return this;
}

void Majo_dtor(Majo* this) {
    Mahoushoujo_dtor((Mahoushoujo*)this);
}

void Majo_attack(Majo* this, Entity* enemy) {
    ((Entity*)enemy)->hp -= this->atk;
}

void Majo_kekkai(Majo* this, Shoujo* sj) {
    sj->kimoji -= 100;
}

void Majo_despair(void* this) {
    return;
}

Majo* mhsj_to_mj(Mahoushoujo* mhsj) {
    assert(mhsj != NULL);
    Majo* majo = (Majo*)malloc(sizeof(Majo));
    Majo_ctor(majo, ((Shoujo*)mhsj)->name, ((Shoujo*)mhsj)->wish);
    Mahoushoujo_dtor(mhsj);
    free(mhsj);
    return majo;
}
// #endregion

// #region Skills
void Madoka_skill(void* this, void* target) {
    puts("Madoka become god, end.\n");
    exit(0);
}

void Homura_skill(void* this, void* target) {
    if (((Entity*)this)->hp < 50) {
        puts("This round is hopeless, Homura go to next round.\n");
        exit(0);
    }
}

void Sayaka_skill(void* this, void* target) {
    ((Entity*)this)->hp += 30;
    ((Shoujo*)this)->kimoji -= 30;
}

void Kyoko_skill(void* this, void* target) {
    if (strcmp(((Shoujo*)target)->name, "Sayaka") == 0) {
        ((Entity*)this)->hp = 0;
        ((Entity*)target)->hp = 0;
    }
}
// #endregion
