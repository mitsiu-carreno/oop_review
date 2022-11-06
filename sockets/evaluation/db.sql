CREATE TABLE up(
    up char(8) primary key not null
);

INSERT INTO up(up) VALUES ('UP200994'),('UP200953'),('UP200667')

CREATE TABLE logs(
    up char(8), 
    proto char(3) not null, 
    port int not null, 
    bytes_in smallint not null, 
    end_signal varchar(100) not null, 
    bytes_out smallint not null, 
    msg bytea, 
    date timestamp DEFAULT CURRENT_TIMESTAMP, 
    CONSTRAINT fk_up FOREIGN KEY(up) REFERENCES up(up)
);
