#include "enemy.h"
#include <iostream> 
#include <cmath>
#include <time.h>
enum eTypes { MOVELEFT = 0, FOLLOW, FOLLOWFAST, FOLLOWSHOOT, FOLLOWFASTSHOOT };

Enemy::Enemy(Texture* texture, Vector2u windowBounds,
    Vector2f position, Vector2f direction,
    Vector2f scale, EnemyType type, Texture* projectileTexture,
    int hpMax, int damageMax, int damageMin)
    :texture(texture), projectileTexture(projectileTexture), type(type),
    direction(direction)
{
    sprite.setTexture(*texture);
    sprite.setScale(scale);
    sprite.setPosition(position);

    


    // ORIGIN sprite'a (œrodek)
    sprite.setOrigin(
        sprite.getLocalBounds().width / 2.f,
        sprite.getLocalBounds().height / 2.f
    );

    float hitboxScale_h = 0.5f;
    float hitboxScale_w = 0.5f;

    //hitboxy
    switch (type)
    {
    case EnemyType::Harpy:
        hitboxScale_h = HARPY_HITBOX_SCALE_H;
        hitboxScale_w = HARPY_HITBOX_SCALE_W;
        break;
    case EnemyType::Raven:
        hitboxScale_h = RAVEN_HITBOX_SCALE_H;
        hitboxScale_w = RAVEN_HITBOX_SCALE_W;
        break;
    case EnemyType::Tank:
        hitboxScale_h = TANK_HITBOX_SCALE_H;
        hitboxScale_w = TANK_HITBOX_SCALE_W;
        break;
    case EnemyType::Wraith:
        hitboxScale_h = WRAITH_HITBOX_SCALE_H;
        hitboxScale_w = WRAITH_HITBOX_SCALE_W;
        break;
    case EnemyType::Argus:
        hitboxScale_h = Argus_HITBOX_SCALE_H;
        hitboxScale_w = Argus_HITBOX_SCALE_W;
        break;
    case EnemyType::BossBayle:
        hitboxScale_h = Bayle_HITBOX_SCALE_H;
        hitboxScale_w = Bayle_HITBOX_SCALE_W;
        break;
    }

    // STA£Y ROZMIAR HITBOXA
    hitbox.setSize({
     sprite.getLocalBounds().width * hitboxScale_w,
     sprite.getLocalBounds().height * hitboxScale_h
        });

    hitbox.setOrigin(
        hitbox.getSize().x / 2.f,
        hitbox.getSize().y / 2.f
    );

    // DEBUG
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2.f);


    this->texture = texture;
    this->sprite.setTexture(*texture);
    this->sprite.setScale(scale);
    this->sprite.setPosition(position);

    this->projectileTexture = projectileTexture;

    this->type = type;
    this->direction = direction;

    this->scale = scale;
    this->sprite.setScale(this->scale);

    this->startX = position.x;
    this->stopY = windowBounds.y * 0.6f;  // wysokoœæ, na której Harpia "zawisa"



    // Statystyki 
    switch (type)
    {
    case EnemyType::Harpy:
        this->shootTimerMax = 60.f;
        this->projectileSpeed = 2.f;
        this->speed = 1.f;
        this->stopY = windowBounds.y * 0.2f;
        this->hpMax = (hpMax > 0) ? hpMax : 3;
        this->damageMax = (damageMax > 0) ? damageMax : 2;
        this->damageMin = (damageMin > 0) ? damageMin : 1;
        this->scale = Vector2f(0.03f, 0.06f);//pocisk
        break;

    case EnemyType::Raven:
        this->speed = 2.f;
        this->hpMax = (hpMax > 0) ? hpMax : 2;
        this->damageMax = (damageMax > 0) ? damageMax : 3;
        this->damageMin = (damageMin > 0) ? damageMin : 2;
        break;

    case EnemyType::Argus:
        this->speed = 1.5f;
        this->startX = position.x;
        this->amplitude = 10.f;
        this->frequency = 2.f;
        this->hpMax = (hpMax > 0) ? hpMax : 3;
        this->damageMax = (damageMax > 0) ? damageMax : 2;
        this->damageMin = (damageMin > 0) ? damageMin : 1;
        break;
    case EnemyType::Wraith:
        this->shootTimerMax = 200;    // rzadko strzela
        this->projectileSpeed = 3.f;
        this->speed = 1.f;
        this->hpMax = 2;               // ma³o HP
        this->damageMin = 1;
        this->damageMax = 2;
        this->stopY = windowBounds.y * 0.2f; // jak Harpy
        this->scale = Vector2f(0.03f, 0.06f);
        break;  
    case EnemyType::Tank:
        this->sprite.setScale(Vector2f(0.4f, 0.4f));
        this->speed = 0.5f;
        this->hpMax = 20;
        this->damageMax = (damageMax > 0) ? damageMax : 3;
        this->damageMin = (damageMin > 0) ? damageMin : 2;
        break;
    case EnemyType::BossBayle:
        this->hpMax = 100;
        this->stopY = windowBounds.y * 0.2f;

        this->sprite.setScale(Vector2f(2.f, 2.f));
        this->animRect.left = 0;
        this->animRect.top = 0;
        this->animRect.width = texture->getSize().x / frameCount;
        this->animRect.height = texture->getSize().y;

        sprite.setTextureRect(animRect);

        sprite.setOrigin(
            animRect.width / 2.f,
            animRect.height / 2.f
        );
           this->startPos = position; // zapamiêtaj pozycjê startow¹
        this->currentAttack = BossAttackType::None;
        isAttacking = false;
        this->attackTimer = 0.f;
        this->attackCooldown = 3.f;
        break;

    }

    this->hp = this->hpMax;
}

Enemy::~Enemy()
{

}



void Enemy::handleVerticalShot(float dt, const sf::Vector2f& playerPos)
{
    static float shotTimer = 0.f;
    shotTimer += dt;

    float shotInterval = 0.3f; // strza³ co pó³ sekundy
    if (shotTimer >= shotInterval)
    {
        shotTimer = 0.f;

        // Pozycja spawn pocisku — ze œrodka sprite, uwzglêdniaj¹c skalê
        sf::Vector2f spawnPos = sprite.getPosition() +
            sf::Vector2f(sprite.getGlobalBounds().width * 0.01f * sprite.getScale().x,
                sprite.getGlobalBounds().height * -0.01f * sprite.getScale().y);


        // Kierunek w stronê gracza
        sf::Vector2f dir = playerPos - spawnPos;

        dir.x += 125.f;

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0.f)
            dir /= len; // normalizacja

        // Prêdkoœæ pocisku — ³atwo zmieniæ
        float projectileSpeed = 6.f;

        // Skala pocisku — ³atwo zmieniæ
        sf::Vector2f projectileScale(0.05f, 0.05f);

        // Dodajemy pocisk do wektora
        fires.emplace_back(projectileTexture, spawnPos, dir * projectileSpeed, projectileScale);
    }

    attackTimer += dt;
    if (attackTimer >= 10.f)
    {
        attackTimer = 0.f;
        isAttacking = false;
        currentAttack = BossAttackType::None;
    }
}



void Enemy::handleCrossShot(float dt)
{
    static float shotTimer = 0.f;
    shotTimer += dt;

    float shotInterval = 0.3f;
    if (shotTimer >= shotInterval)
    {
        shotTimer = 0.f;

        Vector2f spawnPos = sprite.getPosition();
        Vector2f projectileScale(0.05f, 0.05f);

        // 4 losowe kierunki
        for (int i = 0; i < 4; i++)
        {
            float angle = static_cast<float>(rand() % 360) * 3.14159265f / 180.f;
            Vector2f dir(std::cos(angle), std::sin(angle));
            fires.emplace_back(projectileTexture, spawnPos, dir * 5.f, Vector2f(0.05f, 0.2f));
        }
    }

    attackTimer += dt;
    if (attackTimer >= 3.5f)
    {
        attackTimer = 0.f;
        isAttacking = false;
        currentAttack = BossAttackType::None;
    }
}

void Enemy::handleSpiralShot(float dt)
{
    static float shotTimer = 0.f;
    static float elapsed = 0.f; // do sinusoidy

    shotTimer += dt;
    float shotInterval = 0.1f;

    if (shotTimer >= shotInterval)
    {
        shotTimer = 0.f;

        Vector2f spawnPos = sprite.getPosition();
        Vector2f projectileScale(0.05f, 0.05f);
        float speed = 6.f;

        // pociski lec¹ w dó³ ze sinusoidalnym przesuniêciem w X
        float xOffset = std::sin(elapsed * 5.f) * 2.f;
        Vector2f dir(xOffset, 1.f); // Y w dó³
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0.f) dir /= len;

        fires.emplace_back(projectileTexture, spawnPos, dir * speed, Vector2f(0.05f, 0.2f));

        elapsed += 0.2f;
    }

    attackTimer += dt;
    if (attackTimer >= 5.f)
    {
        attackTimer = 0.f;
        isAttacking = false;
        currentAttack = BossAttackType::None;
    }
}




void Enemy::updateAnimation(float dt)
{
    animTimer += dt;
    if (animTimer >= animSpeed)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
        animRect.left = animRect.width * currentFrame;
        sprite.setTextureRect(animRect);
    }
}




void Enemy::takeDamage(int damage)
{
    this->hp -= damage;

    if (this->hp <= 0)
        this->hp = 0;
}

//rotacja raven'a i tanka
void Enemy::updateRotation(const Vector2f& targetPosition)
{
    // Obliczamy kierunek do celu
    Vector2f dir = targetPosition - this->sprite.getPosition();

    // Normalizacja
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0.f)
        dir /= length;

    // Obrót w kierunku ruchu
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;

    // Jeœli sprite domyœlnie "patrzy w górê", dodaj 90 stopni
    this->sprite.setRotation(angle + 90.f);

    // Zaktualizuj kierunek ruchu
    this->direction = dir;
}



void Enemy::Update(const sf::Vector2f& playerPos, float dt)
// Wypisuj pozycjê co klatkê (tymczasowo)
 //std::cout << "Enemy X: " << this->sprite.getPosition().x << " Y: " << this->sprite.getPosition().y << std::endl;
{

    if (type == EnemyType::Harpy)
    {
        // Zatrzymaj siê na 60% wysokoœci ekranu
        if (!reachedStopY)
        {
            sprite.move(0.f, speed);

            if (sprite.getPosition().y >= stopY)
            {
                reachedStopY = true;
                startX = sprite.getPosition().x; // wa¿ne
                elapsedTime = 0.f;               // reset sinusa
            }
        }
        else
        {
            this->elapsedTime += 1.f / 60.f;
            float xOffset = std::sin(this->elapsedTime * 0.8f) * 40.f;
            float yOffset = std::cos(this->elapsedTime * 1.5f) * 20.f;

            this->sprite.setPosition(
                this->startX + xOffset,
                this->stopY + yOffset
            );

            // Liczymy czas do strza³u
            shootTimer++;
            if (shootTimer >= shootTimerMax)
            {
                shootTimer = 0; // reset licznika

                // Tworzymy pocisk w kierunku gracza
                sf::Vector2f dir = playerPos - sprite.getPosition();
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0.f) dir /= len;
                Vector2f spawnPos = sprite.getPosition();
                spawnPos.x += sprite.getGlobalBounds().width / 2.f;
                spawnPos.y += sprite.getGlobalBounds().height / 2.f;

                Vector2f offset(sprite.getGlobalBounds().width * 0.9f, sprite.getGlobalBounds().height * 0.8f);// resp pocisku
                fires.emplace_back(projectileTexture, sprite.getPosition() + offset, dir * projectileSpeed, scale);// dir predkosc

            }
        }
        hitbox.setPosition(sprite.getPosition());
        hitbox.setRotation(sprite.getRotation());
    }
    else if (type == EnemyType::Raven || type == EnemyType::Tank)
    {
        sf::Vector2f toPlayer = playerPos - sprite.getPosition();

        // normalizacja wektora kierunku
        float len = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
        if (len != 0.f)
            toPlayer /= len;

        // ruch w stronê gracza (speed*2)
        sprite.move(toPlayer * speed * 2.f);

        // obrót sprite'a w kierunku ruchu
        float angle = std::atan2(toPlayer.y, toPlayer.x) * 180.f / 3.14159265f;

        // jeœli sprite domyœlnie patrzy w górê, dodaj 90 stopni
        sprite.setRotation(angle + 90.f);
    }
    else if (type == EnemyType::Argus)
    {
        elapsedTime += 1.f / 60.f; // lub deltaTime jeœli masz
        sf::Vector2f toPlayer = playerPos - sprite.getPosition();
        float len = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
        if (len != 0.f) toPlayer /= len;

        // Y idzie w kierunku gracza
        float moveY = toPlayer.y * speed;

        // X pod¹¿a za graczem + sinus
        float moveX = toPlayer.x * speed + amplitude * std::sin(frequency * elapsedTime);

        sprite.move(moveX, moveY);
    }
    else if (type == EnemyType::Wraith)
    {
        if (!reachedStopY)
        {
            sprite.move(0.f, speed);
            if (sprite.getPosition().y >= stopY)
            {
                reachedStopY = true;
                startX = sprite.getPosition().x;
                elapsedTime = 0.f;
            }
        }
        else
        {
            elapsedTime += 1.f / 60.f; // albo deltaTime jeœli masz
            float xOffset = std::sin(elapsedTime * 0.8f) * 40.f;
            float yOffset = std::cos(elapsedTime * 1.5f) * 20.f;
            sprite.setPosition(startX + xOffset, stopY + yOffset);

            // Strza³ w kierunku gracza (rzadko)
            shootTimer++;
            if (shootTimer >= shootTimerMax)
            {
                shootTimer = 0;

                // kierunek w stronê gracza
                sf::Vector2f dir = playerPos - sprite.getPosition();
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0.f) dir /= len;

                // pozycja spawn pocisku
                Vector2f offset(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                // dodajemy pocisk
                fires.emplace_back(
                    projectileTexture,
                    sprite.getPosition() + offset,
                    dir * projectileSpeed,
                    scale
                );
            }
        }
    }
    else if (type == EnemyType::BossBayle)
    {
        updateAnimation(1.f / 60.f);
        // Zatrzymaj siê na 60% wysokoœci ekranu
        if (!reachedStopY)
        {
            sprite.move(0.f, speed);

            if (sprite.getPosition().y >= stopY)
            {
                reachedStopY = true;
                startX = sprite.getPosition().x; // wa¿ne
                elapsedTime = 0.f;               // reset sinusa
            }
        }
        else
        {
            this->elapsedTime += 1.f / 60.f;
            float xOffset = std::sin(this->elapsedTime * 0.8f) * 40.f;
            float yOffset = std::cos(this->elapsedTime * 1.5f) * 20.f;

            this->sprite.setPosition(
                this->startX + xOffset,
                this->stopY + yOffset);

                this->attackTimer += dt;

            // jeœli nie atakuje i min¹³ cooldown, wybierz losowy atak
            if (!isAttacking && this->attackTimer >= this->attackCooldown)
            {
                int r = rand() % 3;
               
                this->currentAttack = static_cast<BossAttackType>(r + 1); // +1 bo None = 0
                isAttacking = true;
                this->attackTimer = 0.f;

                // losowy cooldown 3–5s
                this->attackCooldown = 3.f + static_cast<float>(rand() % 3);
            }

            // wykonanie ataku
            switch (this->currentAttack)
            {
            case Enemy::BossAttackType::VerticalShot:
                handleVerticalShot(dt, playerPos);
                break;
            case Enemy::BossAttackType::CrossShot:
                handleCrossShot(dt);
                break;
            case Enemy::BossAttackType::SpiralShot:
                handleSpiralShot(dt);
                break;
            default:
                break;
            }

           
        }
       
    }
    hitbox.setPosition(sprite.getPosition());
    hitbox.setRotation(sprite.getRotation());
}
void Enemy::Draw(RenderTarget& target)
{

    target.draw(sprite);

   
    
    target.draw(hitbox);
}



