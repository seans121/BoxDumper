#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <3ds.h>
#include <time.h>
#include "../BoxDumper.h"


Result http_download(httpcContext *context)//This error handling needs updated with proper text printing once ctrulib itself supports that.
{
	u32 statuscode = 0;

	httpcBeginRequest(context);
	httpcGetResponseStatusCode(context, &statuscode, 0);

	if (statuscode != 200) return -2;
	return 0;
}


char *base64encode(const char *input)
{
    int      len      = strlen(input);
    int      leftover = len % 3;
    char    *ret      = malloc(((len/3) * 4) + ((leftover)?4:0) + 1);
    int      n        = 0;
    int      outlen   = 0;
    uint8_t  i        = 0;
    uint8_t *inp      = (uint8_t *) input;
    const char *index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

    if (ret == NULL)
        return NULL;

    // Convert each 3 bytes of input to 4 bytes of output.
    len -= leftover;
    for (n = 0; n < len; n+=3) {
        i = inp[n] >> 2;
        ret[outlen++] = index[i];

        i  = (inp[n]   & 0x03) << 4;
        i |= (inp[n+1] & 0xf0) >> 4;
        ret[outlen++] = index[i];

        i  = ((inp[n+1] & 0x0f) << 2);
        i |= ((inp[n+2] & 0xc0) >> 6);
        ret[outlen++] = index[i];

        i  = (inp[n+2] & 0x3f);
        ret[outlen++] = index[i];
    }

    // Handle leftover 1 or 2 bytes.
    if (leftover) {
        i = (inp[n] >> 2);
        ret[outlen++] = index[i];

        i = (inp[n]   & 0x03) << 4;
        if (leftover == 2) {
            i |= (inp[n+1] & 0xf0) >> 4;
            ret[outlen++] = index[i];

            i  = ((inp[n+1] & 0x0f) << 2);
        }
        ret[outlen++] = index[i];
        ret[outlen++] = '=';
        if (leftover == 1)
            ret[outlen++] = '=';
    }
    ret[outlen] = '\0';
    return ret;
}

/*
 * Pokemon slot selection state
 */

void 	reloadPokemon(t_stinf *state)
{
  loadPokemon(state, state->pkmSlot, (u8 *) &state->pkm.pkx);
  pkmRecalc(&state->pkm);
}

void 	uploadBoxesInit(t_stinf *state)
{
  consoleSelect(&state->pch->top);
  consoleClear();

  if (state->inState)
  {
    state->inState = 0;
    reloadPokemon(state);
  }
}

void 	uploadBoxesDisplay(t_stinf *state)
{
	struct s_pkx *pkx = &state->pkm.pkx;
	char data[10240];
	int pokemon = 0;
	int first = 1;
	int currentBox = -1;
	int req = 0;
	int sv = 0;
	//char tmp[50];
	char genderstr[3][8] = {"Male", "Female", "None"};
	char hptable[18][113] = {"Normal", "Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel", "Fire", "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", "Fairy"};
	u8 hptype;
	srand(time(NULL));
	int session = rand();

	printf("Sending Pokemon to the server...\r\n");
	strcpy(data, "{\"pokemon\":{");
	
	for (int i = 0; i != 930; i++) {
		state->pkmSlot = i;
		reloadPokemon(state);
		if (state->pkm.pkx.species) {
			uint32_t IV32 = state->pkm.pkx.individualValues;
			sv = ((pkx->personalityID >> 16) ^ (pkx->personalityID & 0xFFFF)) >> 4;
			bool isEgg = ((IV32 >> 30) & 1);
			hptype = (15 * ((getPkmIV(state->pkm.pkx.individualValues, 0) & 1) + 2 * (getPkmIV(state->pkm.pkx.individualValues, 1) & 1) + 4 * (getPkmIV(state->pkm.pkx.individualValues, 2) & 1) + 8 * (getPkmIV(state->pkm.pkx.individualValues, 3) & 1) + 16 * (getPkmIV(state->pkm.pkx.individualValues, 4) & 1) + 32 * (getPkmIV(state->pkm.pkx.individualValues, 5) & 1))) / 63 + 1;
			pokemon++;

			if (currentBox != state->pkmSlot / 30 + 1) {
				snprintf(data, sizeof data, "%s%s\"box%d\":{",
				data,
				(first ? "" : "},"),
				state->pkmSlot / 30 + 1);
			} else {
				strcat(data, ",");
			}
		
			currentBox = state->pkmSlot / 30 + 1;

			snprintf(data, sizeof data, 
				"%s" // append
				"\"%d\":[" // pokedex id
				"{\"id\":\"%d\", " // pokedex id
				"\"species\":\"%s\", " // species
				"\"gender\":\"%s\", " // gender
				"\"ball\":\"%s\", " // ball 
				"\"ability\":\"%s\", " // ability
				"\"nature\":\"%s\", " // nature
				"\"ot\":\"\", " // OT
				"\"tid\":\"%d\", " // TID
				"\"move1\":\"%s\", " // move 1
				"\"move2\":\"%s\", " // move 2
				"\"move3\":\"%s\", " // move 3
				"\"move4\":\"%s\", " // move 3
				"\"hpIV\":\"%d\", " // HP IV
				"\"attIV\":\"%d\", " // ATT IV
				"\"defIV\":\"%d\", " // DEF IV
				"\"spaIV\":\"%d\", " // SPA IV
				"\"spdIV\":\"%d\", " // SPD IV
				"\"speIV\":\"%d\", " // SPE IV
				"\"hpEV\":\"%d\", " // HP EV
				"\"attEV\":\"%d\", " // ATT EV
				"\"defEV\":\"%d\", " // DEF EV
				"\"spaEV\":\"%d\", " // SPA EV
				"\"spdEV\":\"%d\", " // SPD EV
				"\"speEV\":\"%d\", " // SPE EV
				"\"hp\":\"%s\", " // hp
				"\"egg\":\"%d\", " // is egg
				"\"sv\":\"%04d\"" // SV
				"}]",
				data,
				state->pkmSlot % 30 + 1,
				state->pkm.pkx.species, // pokedex id
				pkData.species[state->pkm.pkx.species], // species
				genderstr[state->pkm.gender], // gender
				pkData.balls[pkx->ballType], // ball
				pkData.abilities[pkx->ability], // ability
				pkData.natures[pkx->nature], // nature
				pkx->trainerID, // TID
				pkData.moves[state->pkm.pkx.moves[0]], // move 1
				pkData.moves[state->pkm.pkx.moves[1]], // move 2
				pkData.moves[state->pkm.pkx.moves[2]], // move 3
				pkData.moves[state->pkm.pkx.moves[3]], // move 3
				getPkmIV(state->pkm.pkx.individualValues, 0), // HP IV
				getPkmIV(state->pkm.pkx.individualValues, 1), // ATT IV
				getPkmIV(state->pkm.pkx.individualValues, 2), // DEF IV
				getPkmIV(state->pkm.pkx.individualValues, 4), // SPA IV
				getPkmIV(state->pkm.pkx.individualValues, 5), // SPD IV
				getPkmIV(state->pkm.pkx.individualValues, 3), // SPE IV
				state->pkm.pkx.effortValues[0], // HP EV
				state->pkm.pkx.effortValues[1], // ATT EV
				state->pkm.pkx.effortValues[2], // DEF EV
				state->pkm.pkx.effortValues[4], // SPA EV
				state->pkm.pkx.effortValues[5], // SPD EV
				state->pkm.pkx.effortValues[3], // SPE EV
				hptable[hptype],
				(isEgg ? 1 : 0), // is egg 11
				sv // SV 14
			);
			
			if (req == 5) {
				snprintf(data, sizeof data, "http://www.boxviewer.xyz/data/upload/?sess=%d&data=%s", session, base64encode(data));
				httpcContext context;
				httpcInit();	
				httpcOpenContext(&context, data, 1);
				http_download(&context);
				httpcCloseContext(&context);
				httpcExit();
				memset(data, 0, 255);
				req = 0;
			}
			req++;
			if (first) first = 0;
		}
		
		printf("%d%% complete\r", i * 100 / 930);
		
	}
	//getOTName(tmp, &state->pkm), // OT
	strcat(data, "}, \"app\":[{\"ver\":\"1.1\"}]}}");
	snprintf(data, sizeof data, "http://www.boxviewer.xyz/data/upload/?sess=%d&data=%s", session, base64encode(data));
	httpcContext context;
	httpcInit();	
	httpcOpenContext(&context, data, 1);
	http_download(&context);
	httpcCloseContext(&context);
	httpcExit();
	printf("100%% complete\n");
	printf("Successfully sent %d Pokemon to the server\n", pokemon);
	printf("\n--------------------------\n");
	state->cont = 0;
}

void 	uploadBoxesInput(t_stinf *state)
{
  u32 	kPressed = state->kPressed;

  if (kPressed & KEY_START)
  {
    state->cont = 0;
    return;
  }

}

struct s_UIState uploadBoxesState = {&uploadBoxesInit, &uploadBoxesDisplay, &uploadBoxesInput};
