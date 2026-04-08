-- Удаление старых таблиц (если есть)
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS orders;
DROP TABLE IF EXISTS cars;
DROP TABLE IF EXISTS drivers;

-- Таблица cars (автомобили)
CREATE TABLE cars (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    car_number TEXT UNIQUE,
    brand TEXT,
    mileage_on_purchase INTEGER DEFAULT 0,
    capacity REAL CHECK (capacity > 0)
);

-- Таблица drivers (водители)
CREATE TABLE drivers (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    personnel_number TEXT UNIQUE,
    last_name TEXT,
    category TEXT,
    experience INTEGER CHECK (experience >= 0),
    address TEXT,
    birth_year INTEGER CHECK (birth_year > 1920)
);

-- Таблица orders (заказы)
CREATE TABLE orders (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    order_date TEXT,
    drivers_id INTEGER NOT NULL,
    cars_id INTEGER NOT NULL,
    mileage INTEGER CHECK (mileage > 0),
    cargo_mass REAL CHECK (cargo_mass > 0),
    transport_cost REAL CHECK (transport_cost >= 0),
    FOREIGN KEY (drivers_id) REFERENCES drivers(id) ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (cars_id) REFERENCES cars(id) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Таблица users (пользователи)
CREATE TABLE users (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE,
    password TEXT,
    role TEXT DEFAULT 'operator',
    drivers_id INTEGER,
    FOREIGN KEY (drivers_id) REFERENCES drivers(id) ON DELETE SET NULL ON UPDATE CASCADE
);

-- Вставка данных: водители
INSERT INTO drivers (personnel_number, last_name, category, experience, address, birth_year) VALUES 
('001', 'Иванов Иван Иванович', 'E', 15, 'ул. Ленина, д.1', 1980),
('002', 'Петров Петр Петрович', 'C', 8, 'ул. Гагарина, д.10', 1990),
('003', 'Сидоров Сидор Сидорович', 'D', 20, 'пр. Мира, д.7', 1975),
('004', 'Козлова Анна Сергеевна', 'B', 3, 'ул. Садовая, д.3', 1998),
('005', 'Смирнов Алексей Владимирович', 'E', 12, 'ул. Лесная, д.8', 1985),
('006', 'Кузнецов Дмитрий Александрович', 'C', 5, 'ул. Цветочная, д.2', 1992),
('007', 'Попова Елена Михайловна', 'D', 10, 'ул. Речная, д.15', 1982);

-- Вставка данных: автомобили
INSERT INTO cars (car_number, brand, mileage_on_purchase, capacity) VALUES 
('А123ВС', 'КАМАЗ-5490', 12000, 20.0),
('В456ЕК', 'ГАЗ-3302', 5000, 1.5),
('С789НМ', 'МАЗ-5440', 25000, 25.0),
('Х012РТ', 'Volvo FH', 8000, 22.0),
('Т345ОР', 'Scania R440', 15000, 24.0),
('У678АВ', 'КАМАЗ-6520', 3000, 30.0),
('К901МН', 'ГАЗ-2705', 10000, 1.2);

-- Вставка данных: заказы
INSERT INTO orders (order_date, drivers_id, cars_id, mileage, cargo_mass, transport_cost) VALUES 
('2024-03-01', 1, 1, 500, 15.0, 30000),
('2024-03-03', 2, 2, 120, 1.2, 5000),
('2024-03-05', 3, 3, 800, 20.0, 45000),
('2024-03-07', 1, 1, 300, 18.0, 25000),
('2024-03-10', 4, 2, 80, 1.0, 3500),
('2024-03-12', 5, 4, 650, 19.0, 40000),
('2024-03-15', 2, 5, 400, 22.0, 35000),
('2024-03-18', 6, 6, 900, 28.0, 60000),
('2024-03-20', 3, 3, 550, 23.0, 48000),
('2024-03-22', 7, 7, 150, 1.0, 4200),
('2024-03-25', 1, 4, 700, 20.0, 42000),
('2024-03-28', 5, 1, 350, 16.0, 28000);

-- Вставка данных: пользователи
INSERT INTO users (username, password, role, drivers_id) VALUES 
('admin', 'admin123', 'admin', NULL),
('ivanov', 'pass123', 'operator', 1),
('petrov', 'pass456', 'operator', 2),
('sidorov', 'pass789', 'operator', 3),
('kozlov', 'pass000', 'operator', 4),
('smirnov', 'pass111', 'operator', 5),
('kuznetsov', 'pass222', 'operator', 6),
('popova', 'pass333', 'operator', 7);