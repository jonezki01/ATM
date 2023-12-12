CREATE TABLE `card` (
  `idcard` int NOT NULL AUTO_INCREMENT,
  `debit` tinyint DEFAULT NULL,
  `credit` tinyint DEFAULT NULL,
  `multicard` tinyint DEFAULT NULL,
  `pin` varchar(255) NOT NULL,
  `Customer_idCustomer` int NOT NULL,
  PRIMARY KEY (`idcard`),
  KEY `fk_card_Customer1_idx` (`Customer_idCustomer`),
  CONSTRAINT `fk_card_Customer1` FOREIGN KEY (`Customer_idCustomer`) REFERENCES `customer` (`idCustomer`)
)
CREATE TABLE `transaction` (
  `idtransaction` int NOT NULL AUTO_INCREMENT,
  `action` varchar(45) DEFAULT NULL,
  `sum` float DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT NULL,
  `cardaccountid` int DEFAULT NULL,
  PRIMARY KEY (`idtransaction`),
  KEY `fk_transaction_account1_idx1` (`cardaccountid`),
  CONSTRAINT `fk_transaction_account1` FOREIGN KEY (`cardaccountid`) REFERENCES `card_has_account` (`idcardhasaccount`)
)
CREATE TABLE `account` (
  `idaccount` int NOT NULL AUTO_INCREMENT,
  `balance` float DEFAULT NULL,
  `credit_limit` float DEFAULT NULL,
  `Customer_idCustomer` int NOT NULL,
  PRIMARY KEY (`idaccount`),
  KEY `fk_account_Customer_idx` (`Customer_idCustomer`),
  CONSTRAINT `fk_account_Customer` FOREIGN KEY (`Customer_idCustomer`) REFERENCES `customer` (`idCustomer`)
)
CREATE TABLE `card_has_account` (
  `idcardhasaccount` int NOT NULL AUTO_INCREMENT,
  `card_idcard` int NOT NULL,
  `account_idaccount` int NOT NULL,
  PRIMARY KEY (`idcardhasaccount`),
  KEY `fk_card_has_account_account1_idx` (`account_idaccount`),
  KEY `fk_card_has_account_card1_idx` (`card_idcard`),
  CONSTRAINT `fk_card_has_account_account1` FOREIGN KEY (`account_idaccount`) REFERENCES `account` (`idaccount`),
  CONSTRAINT `fk_card_has_account_card1` FOREIGN KEY (`card_idcard`) REFERENCES `card` (`idcard`)
)
CREATE TABLE `customer` (
  `idCustomer` int NOT NULL AUTO_INCREMENT,
  `fname` varchar(45) NOT NULL,
  `lname` varchar(45) NOT NULL,
  `city` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idCustomer`)
)