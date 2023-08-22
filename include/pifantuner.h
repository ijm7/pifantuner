#pragma once

typedef struct pifantuner_s {
		int file;
} pifantuner_t;

typedef struct pifantuner_map_s {
		int temperature;
		int speed;
} pifantuner_map_t;

void pifantuner_poll(const pifantuner_t *pifantuner);

pifantuner_t *pifantuner_create(void);
void pifantuner_destroy(pifantuner_t *pifantuner);
