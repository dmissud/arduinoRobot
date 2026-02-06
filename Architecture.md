# Architecture du projet arduinoRobot

## Vue d'ensemble

Ce projet implémente un système de contrôle robotique pour Arduino utilisant une **architecture à composants** avec un **Singleton** coordinateur. Il permet de contrôler un bras robotique 4-DOF (base, coude, bras, pince) ainsi qu'une LED d'état.

---

## Structure des répertoires

```
arduinoRobot/
├── include/                    # Fichiers d'en-tête
│   ├── Application.h           # Contrôleur d'application
│   ├── CommandParser.h         # Utilitaire de parsing
│   ├── Component.h             # Interface composant
│   ├── LEDController.h         # Contrôle LED
│   ├── RobotSystem.h           # Coordinateur système
│   ├── ServoController.h       # Contrôle servo
│   └── StatusController.h      # Contrôle état
├── src/                        # Implémentations
│   ├── main.cpp                # Point d'entrée
│   ├── Application.cpp         # Logique application
│   ├── RobotSystem.cpp         # Implémentation système
│   ├── ServoController.cpp     # Implémentation servo
│   ├── LEDController.cpp       # Implémentation LED
│   └── StatusController.cpp    # Implémentation état
├── lib/                        # Bibliothèques locales
├── test/                       # Tests
└── platformio.ini              # Configuration build
```

---

## Système de build

### PlatformIO

Le projet utilise **PlatformIO** comme système de build avec support multi-cibles :

| Carte | Plateforme | Baud Rate |
|-------|------------|-----------|
| Arduino Uno | ATmega328P | 9600 |
| Arduino Mega 2560 | ATmega2560 | 115200 |
| ESP8266 NodeMCU | ESP8266 | 115200 |

### Dépendances

- `Wire` - Communication I2C (standard Arduino)
- `Adafruit PWM Servo Driver Library` (v2.4.0+) - Contrôle PWM via I2C

---

## Architecture logicielle

### Diagramme des composants

```
main.cpp
  └─> RobotSystem (Singleton)
      ├─> Application (routeur de commandes)
      │   └─> Component[] (tableau de 10 max)
      │       ├─> StatusController
      │       ├─> LEDController
      │       └─> ServoController (x4 instances)
      └─> Adafruit_PWMServoDriver (contrôleur PWM I2C)
```

### Hiérarchie des classes

```
Component (Interface abstraite)
├── StatusController (final)
├── LEDController (final)
└── ServoController (final)

RobotSystem (Singleton)
├── has-a Application
│   └── has-many Component[] (max 10)
└── has-a Adafruit_PWMServoDriver

CommandParser (Utilitaire statique)
└── produit ParsedCommand struct
```

---

## Composants principaux

### RobotSystem (Singleton)

**Fichiers :** `include/RobotSystem.h`, `src/RobotSystem.cpp`

**Responsabilités :**
- Initialisation du système (Serial, I2C, PWM)
- Réception et parsing des commandes série
- Routage des commandes vers l'Application
- Pattern Singleton (instance unique garantie)

**Méthodes clés :**
- `getInstance()` - Retourne l'instance unique
- `setup()` - Initialise Serial (9600 baud), Wire I2C, PWM (50Hz)
- `processCommands()` - Boucle principale : lecture série, parsing, exécution

### Application (Routeur de commandes)

**Fichiers :** `include/Application.h`, `src/Application.cpp`

**Responsabilités :**
- Maintenir le registre des composants
- Router les commandes vers les composants appropriés
- Gérer les commandes non reconnues

**Méthodes clés :**
- `addComponent(Component*)` - Enregistre un nouveau composant
- `run(ParsedCommand)` - Exécute la commande en trouvant le composant correspondant

### Component (Interface abstraite)

**Fichier :** `include/Component.h`

**Interface :**
```cpp
virtual String getKeywords() const = 0;
virtual bool processCommand(const ParsedCommand&) = 0;
```

**Implémentations :**
- StatusController
- LEDController
- ServoController

### ServoController

**Fichiers :** `include/ServoController.h`, `src/ServoController.cpp`

**Responsabilités :**
- Contrôle individuel des servomoteurs via PWM
- Conversion angles vers largeurs d'impulsion PWM
- Support des opérations de balayage (sweep)

**Configuration :**
- Plage d'impulsion : 100-650 microsecondes
- Plage d'angle : 0-180 degrés

**Instances servos (4 total) :**
| Canal | Nom | Fonction |
|-------|-----|----------|
| 0 | base | Rotation de la base |
| 1 | elbow | Articulation du coude |
| 2 | arm | Extension du bras |
| 3 | claw | Pince/gripper |

### LEDController

**Fichiers :** `include/LEDController.h`, `src/LEDController.cpp`

**Responsabilités :**
- Contrôle ON/OFF de la LED via GPIO

**Configuration :**
- Pin : 12 (sortie digitale)
- Nom : "led"

### StatusController

**Fichiers :** `include/StatusController.h`, `src/StatusController.cpp`

**Responsabilités :**
- Rapporter l'état du système
- Vérification de santé simple

### CommandParser (Utilitaire)

**Fichier :** `include/CommandParser.h`

**Format de commande :**
```
keyword(name, action)
```

**Structure parsée :**
```cpp
struct ParsedCommand {
    String keyword;     // Catégorie (servo, led, status)
    String name;        // Nom de l'instance
    String action;      // Action (on, off, angle100, sweep, ok)
    bool isValid;       // Flag de validation
};
```

---

## Protocole de communication

### Communication série (Entrée)

- **Baud rate :** 9600 (configurable dans platformio.ini)
- **Format :** Lignes terminées par `\n`
- **Syntaxe :** `keyword(name, action)`

### Flux de données

```
Entrée Série (9600 baud)
    ↓
RobotSystem::processCommands() lecture/parsing
    ↓
CommandParser::parse() validation structure
    ↓
Application::run() routage vers composant
    ↓
Component::processCommand() exécution action
    ↓
Sortie Série (réponses)
```

### Exemples de commandes

| Commande | Description |
|----------|-------------|
| `servo(base,angle90)` | Positionne le servo base à 90° |
| `servo(arm,sweep)` | Balayage complet du servo arm |
| `led(led,on)` | Allume la LED |
| `led(led,off)` | Éteint la LED |
| `status(arduino,ok)` | Vérifie l'état (répond "ready") |

### Messages de réponse

- `Servo base positionné à 90 degrés` - Succès servo
- `ready` - Réponse status check
- `Commande invalide : Non executée : <commande>` - Erreur
- `Ok: Pulse positionné à 450 us` - Confirmation PWM

### Communication I2C

**Maître :** Arduino (Uno/Mega/ESP8266)
**Esclave :** Adafruit PWM Servo Driver

**Initialisation :**
```cpp
Wire.begin();           // Initialise I2C en maître
m_pwm.begin();          // Initialise driver PWM
m_pwm.setPWMFreq(50);   // 50Hz pour servos standard
```

---

## Patterns de conception

### 1. Singleton Pattern

**Implémentation :** RobotSystem

- Constructeur privé
- Méthode statique `getInstance()` retournant une instance statique
- Constructeur de copie et opérateur d'assignation supprimés
- Garantit une instance unique dans toute l'application

### 2. Component Pattern (Architecture Plugin)

**Implémentation :** Interface Component + registre Application

- Classe de base Component définissant le contrat
- Implémentations concrètes (StatusController, LEDController, ServoController)
- Application maintient un tableau de composants
- Couplage faible : Application ne connaît pas les types spécifiques

### 3. Strategy Pattern

**Implémentation :** Implémentations Component

- Chaque composant encapsule sa stratégie de contrôle spécifique
- `processCommand()` implémente la stratégie pour ce composant

### 4. Injection de dépendances

**Implémentation :** Constructeur ServoController

- Driver PWM passé par référence (non instancié en interne)
- Réduit le couplage, permet la flexibilité

---

## Configuration matérielle

### Cartes supportées

| Carte | I2C SDA | I2C SCL |
|-------|---------|---------|
| Arduino Uno | A4 | A5 |
| Arduino Mega | 20 | 21 |
| ESP8266 NodeMCU | D1 | D2 |

### Périphériques

- **LED :** Pin 12 (sortie digitale)
- **Servos :** 4 canaux via driver PWM I2C (canaux 0-3)
- **Bus I2C :** Pins SDA/SCL selon la carte

### Contraintes

- Buffer série : Dépend de la plateforme (64-256 octets typiquement)
- Limite composants : 10 maximum
- Fréquence PWM : Fixée à 50Hz (standard servomoteurs)
- Plage d'angle : 0-180 degrés

---

## Exemple de flux de données

**Commande : Positionner un servo**

```
Entrée: "servo(base,angle90)"
    ↓
RobotSystem::processCommands()
    Serial.readStringUntil('\n') → "servo(base,angle90)"
    ↓
CommandParser::parse()
    keyword: "servo"
    name: "base"
    action: "angle90"
    isValid: true
    ↓
Application::run()
    Itère les composants
    Trouve: getKeywords() == "servo" (ServoController pour "base")
    ↓
ServoController::processCommand()
    Vérifie: name == "base" → true
    Vérifie: action.startsWith("angle") → true
    Extrait: angle = 90
    Valide: 0 <= 90 <= 180 → true
    ↓
ServoController::setAngle(90)
    angleToPulse(90) → 375 microsecondes
    m_pwm.setPWM(0, 0, 375)
    Sortie série: "Servo base positionné à 90 degrés"
```

---

## Extensibilité

### Ajouter un nouveau composant

1. Créer une classe héritant de `Component`
2. Implémenter `getKeywords()` et `processCommand()`
3. Instancier et enregistrer dans `RobotSystem::setup()` via `m_application.addComponent()`

### Ajouter un nouveau servo

1. Dans `RobotSystem::setup()`, créer une nouvelle instance de `ServoController`
2. Spécifier le canal PWM et le nom
3. Enregistrer avec `m_application.addComponent()`

---

## Historique de développement

| Commit | Description |
|--------|-------------|
| a4f60c9 | Encapsulation du Robot (Singleton RobotSystem) |
| 853874a | Améliorer validation réponses série et configuration servos |
| 1356ace | Initial commit |
