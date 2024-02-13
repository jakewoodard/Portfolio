import numpy as np
import pandas as pd
import random
import matplotlib.pyplot as plt

SALARY_CAP = 47000
PLAYER_POP = pd.read_csv("week9_data.csv")
name = "LAMAR JACKSON"
PLAYER_POP.loc[PLAYER_POP['NAME'] == name]['DK salary'].values[0]

duplicates = PLAYER_POP.duplicated(subset=['NAME'], keep=False)
PLAYER_POP.drop_duplicates(subset=['NAME'], inplace=True)
for i in duplicates:
    if i == True:
        print(i)

# Define the size of the population and tournament size
POPULATION_SIZE   = 500
TOURNAMENT_SIZE   = 5
MUTATION_RATE     = 0.1
N_ITERATIONS      = 5


# Define the fitness function
def fitness(lineup):
    lineup_rating = 0
    
    # is this lineup stacking a QB & a WR/TE?
    # or inverted stacking which means a QB and a member of 
    # the opposing teams passing game
    QB_team = str(lineup[0]["TEAM"])
    s_count = 0
    inv_count = 0
    
    for player in lineup:
        # check for stack
        if player["POS"] == "WR" or player["POS"] == "TE":
            if player["TEAM"] == QB_team:
                s_count += 1
            if player["OPP"] == QB_team:
                inv_count += 1
                
        # add base rating to lineup
        lineup_rating += player["RTG"]
        # factor in home/away
        if player["h/a"] == "home":
            lineup_rating += 2
        else:
            lineup_rating -= 2
        
        # factor in matchup (def_rtg)
        def_rtg = player["DEF_RTG"]
        lineup_rating += def_rtg
        
    # factor in stack_coeffecient
    if s_count == 1:
        lineup_rating += 10
    if s_count > 1:
        lineup_rating -= ((s_count-1)*5)
    if inv_count == 1:
        lineup_rating += 4
    if inv_count == 2:
        lineup_rating += 0
    if inv_count <= 3:
        lineup_rating -= (4*(inv_count-2))
        
    # never take a tightend at flex
    if lineup[7]["POS"] == "TE":
        lineup_rating -= 20
    # prefer wide recievers
    if lineup[7]["POS"] == "WR":
        lineup_rating += 2
        
    return lineup_rating


# Define the tournament selection function
def tournament_selection(population, fitness_scores, tournament_size):
    selected = []
    for i in range(tournament_size):
        tournament = random.sample(range(len(population)), tournament_size)
        winner = tournament[0]
        for j in tournament:
            if fitness_scores[j] > fitness_scores[winner]:
                winner = j
        selected.append(population[winner])
    return selected

# Define the mutation function
def mutation(lineup, mutation_rate):
    while True:
        mutated = []
        
        # QB
        if random.random() < mutation_rate:
            mutated.append(qb.iloc[random.randint(0, len(qb)-1)])
        else:
            mutated.append(lineup[0])
            
        # RB
        if random.random() < mutation_rate:
            rb1 = rb.iloc[random.randint(0, len(rb)-1)]
            while rb1.equals(lineup[2]) or rb1.equals(lineup[7]): 
                rb1 = rb.iloc[random.randint(0, len(rb)-1)]
            mutated.append(rb1)
        else:
            rb1 = lineup[1]
            mutated.append(rb1)
        if random.random() < mutation_rate:
            rb2 = rb.iloc[random.randint(0, len(rb)-1)]
            while rb2.equals(rb1) or rb2.equals(lineup[7]): 
                rb2 = rb.iloc[random.randint(0, len(rb)-1)]
            mutated.append(rb2)
        else:
            rb2 = lineup[2]
            mutated.append(rb2)
            
        # WR
        if random.random() < mutation_rate:
            wr1 = wr.iloc[random.randint(0, len(wr)-1)]
            while wr1.equals(lineup[4]) or wr1.equals(lineup[5]) or wr1.equals(lineup[7]): 
                wr1 = wr.iloc[random.randint(0, len(wr)-1)]
            mutated.append(wr1)
        else:
            wr1 = lineup[3]
            mutated.append(lineup[3])
        if random.random() < mutation_rate:
            wr2 = wr.iloc[random.randint(0, len(wr)-1)]
            while wr2.equals(wr1) or wr2.equals(lineup[5]) or wr2.equals(lineup[7]):
                wr2 = wr.iloc[random.randint(0, len(wr)-1)]
            mutated.append(wr2)
        else:
            wr2 = lineup[4]
            mutated.append(lineup[4])
        if random.random() < mutation_rate:
            wr3 = wr.iloc[random.randint(0, len(wr)-1)]
            while wr3.equals(wr1) or wr3.equals(wr2) or wr3.equals(lineup[7]):
                wr3 = wr.iloc[random.randint(0, len(wr)-1)]
            mutated.append(wr3)
        else:
            wr3 = lineup[5]
            mutated.append(lineup[5])
        if random.random() < mutation_rate:
            te1 = te.iloc[random.randint(0, len(te)-1)]
            while te1.equals(lineup[7]):
                te1 = te.iloc[random.randint(0, len(te)-1)]
            mutated.append(te1)
        else:
            te1 = lineup[6]
            mutated.append(lineup[6])
        if random.random() < mutation_rate:
            rb_wr_te = flex.iloc[random.randint(0, len(flex)-1)]
            while rb_wr_te.equals(rb1) or rb_wr_te.equals(rb2) or rb_wr_te.equals(wr1) or rb_wr_te.equals(wr2) or rb_wr_te.equals(wr3) or rb_wr_te.equals(te1):
                rb_wr_te = flex.iloc[random.randint(0, len(flex)-1)]
            mutated.append(rb_wr_te)
        else:
            mutated.append(lineup[7])

        tot = sum(player["DK salary"] for player in mutated)
        if tot <= 47000 and tot >= 45000:
            break

    return mutated

# Define the replacement function
def replace_population(population, offspring):
    new_population = population.copy()
    new_population.sort(key=lambda x: fitness(x), reverse=True)
    for i in range(len(offspring)):
        new_population[-(i+1)] = offspring[i]
    return new_population

def sort_population_by_fitness(population, fitness_scores):
    """Sort the population and fitness scores in descending order based on fitness score."""
    sorted_indices = sorted(range(len(fitness_scores)), key=lambda k: fitness_scores[k], reverse=True)
    return [population[i] for i in sorted_indices], [fitness_scores[i] for i in sorted_indices]

def get_top_n_individuals(population, fitness_scores, n):
    """Return the top n individuals and their corresponding fitness scores."""
    P_sorted, F_sorted = sort_population_by_fitness(population, fitness_scores)
    return P_sorted[:n], F_sorted[:n]


# Define the position groups we need
qb = PLAYER_POP[(PLAYER_POP["POS"] == "QB") & (PLAYER_POP["DK salary"] >= 4800)]
rb = PLAYER_POP[(PLAYER_POP["POS"] == "RB") & (PLAYER_POP["DK salary"] > 4000)]
wr = PLAYER_POP[(PLAYER_POP["POS"] == "WR") & (PLAYER_POP["DK salary"] > 3700)]
te = PLAYER_POP[(PLAYER_POP["POS"] == "TE") & (PLAYER_POP["RTG"] >= 70)]
flex = PLAYER_POP[(PLAYER_POP["POS"].isin(["RB", "WR", "TE"])) & (PLAYER_POP["DK salary"] > 4000)]

# Sort the position groups by descending order of DK salary
qb = qb.sort_values("DK salary", ascending=False)
rb = rb.sort_values("DK salary", ascending=False)
wr = wr.sort_values("DK salary", ascending=False)
te = te.sort_values("DK salary", ascending=False)
flex = flex.sort_values("DK salary", ascending=False)

# Create a population of potential solutions
i = 0
population = []
lineup_points = []
while i < POPULATION_SIZE:

    # Select one player from each position group and one additional player from the flex group
    selected_players = []
    
    # Select QB, no need to check
    selected_players.append(qb.iloc[random.randint(0, len(qb)-1)])
    
    # Select the RBs, ensuring that they are different players
    rb1 = rb.iloc[random.randint(0, len(rb)-1)]
    selected_players.append(rb1)
    
    rb2 = rb.iloc[random.randint(0, len(rb)-1)]
    while rb2.equals(rb1):
        rb2 = rb.iloc[random.randint(0, len(rb)-1)]
    selected_players.append(rb2)

    # Select the WRs, ensuring that they are different players
    wr1 = wr.iloc[random.randint(0, len(wr)-1)]
    selected_players.append(wr1)
    
    wr2 = wr.iloc[random.randint(0, len(wr)-1)]
    while wr2.equals(wr1):
        wr2 = wr.iloc[random.randint(0, len(wr)-1)]
    selected_players.append(wr2)

    wr3 = wr.iloc[random.randint(0, len(wr)-1)]
    while wr3.equals(wr1) or wr3.equals(wr2):
        wr3 = wr.iloc[random.randint(0, len(wr)-1)]
    selected_players.append(wr3)

    # Select TE, no need to check
    te1 = te.iloc[random.randint(0, len(te)-1)]
    selected_players.append(te1)

    # Select FLEX, most checking here
    rb_wr_te = flex.iloc[random.randint(0, len(flex)-1)]
    while rb_wr_te.equals(rb1) or rb_wr_te.equals(rb2) or rb_wr_te.equals(wr1) or rb_wr_te.equals(wr2) or rb_wr_te.equals(wr3) or rb_wr_te.equals(te1):
        rb_wr_te = flex.iloc[random.randint(0, len(flex)-1)]
    selected_players.append(rb_wr_te)

    # Check if the total salary is under the cap
    total_salary = sum(selected_player["DK salary"] for selected_player in selected_players)
    
    if total_salary <= 47000 & total_salary >= 45000:    
        # Print the selected lineup
        #print("Lineup {}: \n".format(i))
        i += 1
        p = 0
        for selected_player in selected_players:
            #print(selected_player["NAME"], selected_player["POS"], selected_player["DK salary"])
            p += selected_player["DK points"]
        #print("\n")
        population.append(selected_players)
        lineup_points.append(p)

avg_points_init_pop = np.mean(lineup_points)
print("AVG LINEUP SCORE OF INIT POPULATION: " + str(avg_points_init_pop))


## RUN GENETIC ALGORITHM
for i in range(N_ITERATIONS):
    # eval fitness
    fitness_scores = [fitness(lineup) for lineup in population]
    
    # selection
    selected = tournament_selection(population, fitness_scores, TOURNAMENT_SIZE)

    # Mutation
    offspring = []
    for i in range(0, len(selected)):
        child = mutation(selected[i], MUTATION_RATE)
        offspring.append(child)
    
    # Replace the old population with the new offspring
    population = replace_population(population, offspring)

# best lineups selected
fitness_scores = [fitness(lineup) for lineup in population]
P_sorted, F_sorted = sort_population_by_fitness(population, fitness_scores)
top_n_individuals = P_sorted[:10]
top_n_fitness_scores = F_sorted[:10]

# print final selected lineups
print("final selected lineups:")
print()
total_points = 0
for j in range(0,len(top_n_individuals)):
    total = 0
    points = 0
    print("lineup " + str(j+1))
    for k in range(0,8):
        s = top_n_individuals[j][k][0]
        sal = top_n_individuals[j][k][7]
        p = round(top_n_individuals[j][k][8], 2)
        name = s.ljust(25)
        p_s = str(p).ljust(6)
        print(top_n_individuals[j][k][1], end = ' ')
        print(name, end = ' ')
        print(str(top_n_individuals[j][k][2]).rjust(3), end = ' ')
        print(sal, end = ' ')
        print(p_s)
        total += int(sal)
        points += p
    print("total salary:", end = '                   ')
    print(total)
    print("total points:", end = '                    ')
    x = (47000-total)/400
    points += x
    print(str(round(points,2)).rjust(10))
    total_points += points
    print()

print("avg points of selected lineups")
print(total_points/10)
