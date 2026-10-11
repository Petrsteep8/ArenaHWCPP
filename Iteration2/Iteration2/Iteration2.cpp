#include <iostream>

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;
public:
    Fighter(const char* name, int health, int damage)
        : name(name), health(health), maxHealth(health), damage(damage)
    {
    }

    void TakeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
        std::cout << name << " получает урон!" << std::endl;
    }

    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }

    bool IsAlive() const {
        return health > 0;
    }

    void PrintStatus() const {
        std::cout << name << "  хп: " << health << " " << maxHealth << "\n";
    }
};

void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;
public:
    Game() :
        player("player1", 30, 8),
        enemies{ {"Enemy 1", 12, 3},
                {"Enemy 2", 18, 4},
                {"Enemy 3", 24, 5} },
        currentTarget(nullptr)
    {
    }

    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;
        if (number < 1 || number > 3) {
            return false;
        }
        if (enemies[number - 1].IsAlive()) {
            outTarget = &enemies[number - 1];
            return true;
        }
        return false;
    }

    bool AllEnemiesDefeated() const {
        for (int enemy = 0; enemy < 3; enemy++) {
            if (enemies[enemy].IsAlive()) {
                return false;
            }
        }
        return true;
    }

    void Run() {
        int command;
        while (player.IsAlive() && !AllEnemiesDefeated()) {
            std::cout << "Игрок: " << std::endl;
            PrintFighter(player);
            std::cout << "Враги: " << std::endl;
            for (int i = 0; i < 3; ++i) {
                PrintFighter(enemies[i]);
            }
            if (currentTarget != nullptr) {
                std::cout << "Текущая цель: " << std::endl;
                currentTarget->PrintStatus();
            }
            else {
                std::cout << "цель: не выбрана" << std::endl;
            }
            std::cout << "1 - выбрать цель (затем ввести номер 1-3)" << std::endl;
            std::cout << "2 - атаковать выбранную цель" << std::endl;
            std::cout << "0 - выйти из игры" << std::endl;
            std::cout << "Ваш выбор: ";
            std::cin >> command;

            if (command == 1) {
                int targetNum;
                std::cout << "Введите номер цели (1-3): ";
                std::cin >> targetNum;
                if (TrySelectTarget(targetNum, currentTarget)) {
                    std::cout << "Цель выбрана" << std::endl;
                }
                else {
                    std::cout << "Номер цели отсутствует" << std::endl;
                    currentTarget = nullptr;
                }
            }
            else if (command == 2) {
                if (currentTarget == nullptr) {
                    std::cout << "Выберите цель" << std::endl;
                }
                else {
                    player.Attack(*currentTarget);

                    if (!currentTarget->IsAlive()) {
                        std::cout << "Враг уничтожен" << std::endl;
                        currentTarget = nullptr;
                    }
                    else {
                        currentTarget->Attack(player);
                    }
                }
            }
            else if (command == 0) {
                std::cout << "Вы вышли из игры." << std::endl;
                break;
            }
            else {
                std::cout << "Неизвестная команда. Попробуйте снова." << std::endl;
            }
        }

        if (!player.IsAlive()) {
            std::cout << "Поражение, игрок погиб." << std::endl;
        }
        else if (AllEnemiesDefeated()) {
            std::cout << "Победа. все враги повержены" << std::endl;
        }
        else {
            std::cout << "игрок вышел" << std::endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Game game;
    game.Run();
    return 0;
}