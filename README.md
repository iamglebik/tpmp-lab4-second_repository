# Автопарк

Консольное приложение на C для учета автомобильных перевозок, водителей и заказов.

## Описание

Автопарк осуществляет обслуживание заказов на перевозку грузов, используя свой парк автомобилей и водителей. Водитель, выполнивший заказ, получает 20% от стоимости перевозки.

## Функционал

- Аутентификация (администратор / водитель)
- CRUD операции с водителями, автомобилями, заказами
- Проверка грузоподъемности при создании заказа
- Отчет по зарплатам всех водителей
- Отчет по зарплате за период (все водители)
- Отчет по зарплате за период (конкретный водитель)
- Поиск водителя с минимальным количеством поездок
- Поиск автомобиля с максимальным пробегом
- Сохранение отчетов в базу данных

## Технологии

- Язык: C
- База данных: SQLite3
- Сборка: Make
- CI/CD: GitHub Actions

## Установка

```bash
git clone https://github.com/iamglebik/tpmp-lab4-second_repository.git
cd tpmp-lab4-second_repository
sudo apt-get install sqlite3 libsqlite3-dev
make clean
make
```

## Запуск

```bash
make run
```

Пароль по умолчанию: `admin123`

## Тестирование

```bash
make test
```

## Структура проекта

```
tpmp-lab4-second_repository/
├── src/
│   ├── main.c
│   ├── auth.c
│   ├── menu.c
│   └── database.c
├── includes/
│   ├── auth.h
│   ├── menu.h
│   └── database.h
├── resources/
│   ├── autopark.db
│   └── create_tables.sql
├── tests/
│   ├── test_auth.c
│   ├── test_database.c
│   ├── test_menu.c
│   └── test_main.c
├── bin/
├── obj/
├── Makefile
└── README.md
```

## Статус сборки

[![CI / Build](https://github.com/iamglebik/tpmp-lab4-second_repository/actions/workflows/build.yml/badge.svg)](https://github.com/iamglebik/tpmp-lab4-second_repository/actions/workflows/build.yml)

## Авторы

| Студент | Роль | Выполненные задачи |
|---------|------|-------------------|
| Синяков Глеб | Тимлид | Создание репозиториев, настройка CI/CD, база данных, аутентификация, отчет за период |
| Манкевич Ангелина | Разработчик | CRUD операции, проверка грузоподъемности, отчет по зарплатам, тесты |

## Ссылки

- [Документация (wiki)](https://github.com/fpmi-tpmp2026/tpmp-gr12a-lab4-banka_pertsev/wiki)
- [GitHub Pages](https://iamglebik.github.io/tpmp-lab4-second_repository/)
