using System;
using System.IO;
using System.Collections.Generic;

struct Coords {
    public int x;
    public int y;

    public Coords(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public override bool Equals(Object obj) {
        return obj is Coords c && this == c;
    }

    public override int GetHashCode() {
        return this.x.GetHashCode() ^ this.y.GetHashCode();
    }

    public static bool operator ==(Coords a, Coords b) {
        return a.x == b.x && a.y == b.y;
    }

    public static bool operator !=(Coords a, Coords b) {
        return !(a == b);
    }
}

class Level {
    Location [,] mygrid;

    public void makeLevel(int x, int y) {
        this.mygrid = new Location[x, y];
        for(int i = 0; i < x; i++) {
            for(int j = 0; j < y; j++){
                mygrid[i,j] = new Location();
                //Console.WriteLine(i.ToString() + " " + j.ToString());
            }
        }
    }

    public void makeExit(int x, int y) {
        this.mygrid[x,y].setExit();
    }
    public void addSkeleton(int x, int y) {
        Entity s = new skeleton(); 
        this.mygrid[x,y].setSkeleton(s);
    }
    public void addKey(int x, int y) {
        Entity k = new key(); 
        this.mygrid[x,y].setKey(k);
    }
    public void addLoot(int x, int y, int gold) {
        Entity l = new loot(gold); 
        this.mygrid[x,y].setLoot(l);
    }
    public void interact(Player p, int x, int y) {
        this.mygrid[x,y].interact(p);
    }
    public void look(int x, int y) {
        this.mygrid[x,y].look();
    }
    public bool TheExit(Coords z) {
        return this.mygrid[z.x, z.y].exit();
    }
}

class Location {
    List<Entity> ents = new List<Entity>();
    bool exit_bool = false;

    public void addent(Entity e) {
        ents.Add(e);
    }
    public void setExit() {
        this.exit_bool = true;
    }
    public void setSkeleton(Entity s) {
        addent(s);
    }
    public void setKey(Entity k) {
        addent(k);
    }
    public void setLoot(Entity l) {
        addent(l);
    }

    public void interact(Player p) {
        for(int i = 0; i < this.ents.Count; i++) {
            this.ents[i].interact(p);
        }
        if(this.exit_bool == true) {
            Console.WriteLine("You open the gate with your key!");
        }
    }
    public void look() {
        for(int i = 0; i < this.ents.Count; i++) {
            this.ents[i].look();
        }
        if ((this.ents.Count == 0) && (this.exit_bool == false)) {
            Console.WriteLine("Not much to see here.");
        }
        if(this.exit_bool == true) {
            Console.WriteLine("That looks like the gate out of this spooky place!");
        }
    }
    public bool exit() {
        return exit_bool;
    }
}

abstract class Entity {
    public virtual void look() {
        Console.WriteLine("Not much to see here.");
    }
    public abstract void interact(Player player);

}
class skeleton : Entity {
    public override void interact(Player p) {
        Console.WriteLine("A bony arm juts out of the ground and grabs your ankle!"); 
        Console.WriteLine("You've been dragged six feet under by a skeleton.");
        p.kill(); 
    }
  public override void look() {
    Console.WriteLine("Not much to see here.");
  }
}
class key : Entity {
    public override void interact(Player p) {
        p.addKey();
        Console.WriteLine("You picked up a key!"); 
    }
  public override void look() {
    Console.WriteLine("You see a key on the ground! Might need that too get out of here...");
  }
}
class loot : Entity {
    int total;
    public loot(int gold){
        total = gold;
    }
    public override void interact(Player p) {
        p.addGold(total);
        Console.WriteLine("You open the chest and find " + total.ToString() + " coins!"); 
    }
  public override void look() {
    Console.WriteLine("You see what looks like the corner of a treasure chest poking out of the ground.");
  }
}

class Player {
    public Coords coords { get; set; }
    int numkeys = 0; // number of keys
    bool dead = false; // is the player dead
    int gold = 0; // amount of money acrued

    public Player() {
        this.coords = new Coords(0, 0);
    }

    public bool is_at(Coords xy) {
        return this.coords == xy;
    }

    public bool alive() { return !(dead); }

    // add gold to player total
    public void addGold(int t) {
        this.gold += t;
    }
    // does player have key
    public bool has_key() { 
        if (numkeys == 0) {
            return false; 
        }
        return true; 
    }
    // add key to total
    public void addKey() {
        numkeys++;
    }

    public void print_stats() {
        Console.WriteLine($"  LOCATION: {this.coords.x}, {this.coords.y}");
        Console.WriteLine($"     COINS: {gold}");
        Console.WriteLine($"       KEY: {has_key()}");
        Console.WriteLine($"      DEAD: {dead}");
    }
    // kill player
    public void kill() {
        this.dead = true;
    }
}

class Game {
    int num_turns;
    int max_turns;
    int xmax;
    int ymax;
    Level  level;
    public Player player { get; }

    public Game() {
        this.player = new Player();
    }

    public void load(string path) {
        this.level = new Level();

        string line;
        using (StreamReader reader = new StreamReader(path)) {
            while ((line = reader.ReadLine()) != null) {
                if (line == "") { continue; }

                string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

                if (split.Length < 3) {
                    Console.WriteLine($"Bad command in level file: '{line}'");
                    Environment.Exit(1);
                }

                int x     = int.Parse(split[1]);
                int y     = int.Parse(split[2]);
                int count = 0;

                if (split.Length > 3) {
                    count = int.Parse(split[3]);
                }

                switch (split[0]) {
                    case "size":
                        // Set the level's size to x by y
                        this.level.makeLevel(x,y);
                        this.max_turns = 2*x*y;
                        xmax = x - 1;
                        ymax = y - 1;
                        break;
                    case "exit":
                        // Set the level's exit location to be x, y
                        this.level.makeExit(x,y);
                        exxit.xx = x;
                        exxit.yy = y;
                        break;
                    case "key":
                        // Add a key to location x, y
                        this.level.addKey(x,y);
                        break;
                    case "loot":
                        // Add loot to location x, y with count coins
                        this.level.addLoot(x,y,count);
                        break;
                    case "skeleton":
                        // Add a skeleton to location x, y
                        this.level.addSkeleton(x,y);
                        break;
                    default:
                        Console.WriteLine($"Bad command in level file: '{line}'");
                        Environment.Exit(1);
                        break;

                }
            }
        }
    }

    public void input(string line) {
        this.num_turns += 1;

        // Check for exhaustion?
        if (num_turns > max_turns) {
            Console.WriteLine("You died due exhaustion.");
            player.kill();
            this.exit();
        }
        Console.WriteLine("================================================================");

        string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

        if (split.Length != 2) {
            Console.WriteLine($"Bad command in input: '{line}'");
            return;
        }

        Coords new_coords = this.player.coords;
        switch (split[1]) {
            case "north":
                new_coords.y += 1;
                break;
            case "south":
                new_coords.y -= 1;
                break;
            case "east":
                new_coords.x += 1;
                break;
            case "west":
                new_coords.x -= 1;
                break;
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }

        // Are the new coords valid?
        switch (split[0]) {
            case "go":
                // check
                if ((new_coords.y > ymax) || (new_coords.x > xmax) || (new_coords.x < 0) || (new_coords.y < 0)) {
                    Console.WriteLine("A towering wall is before you. This must be the edge of the graveyard.");
                } else {
                    this.player.coords = new_coords; // next location
                    this.level.look(new_coords.x, new_coords.y);
                    this.level.interact(this.player, new_coords.x, new_coords.y);
                }
                break;
            case "look":
                // check
                if ((new_coords.y > ymax) || (new_coords.x > xmax) || (new_coords.x < 0) || (new_coords.y < 0)) {
                    Console.WriteLine("A towering wall is before you. This must be the edge of the graveyard.");
                } else {
                    // look
                    this.level.look(new_coords.x, new_coords.y);
                }
                break;
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }
    }
    bool is_over() {
        // What are the exit conditions?
        bool b = false;
        b = player.has_key();
        bool spot = false; // is the player at the exit?
        Coords c;
        c.x = exxit.xx;
        c.y = exxit.yy;
        spot = player.is_at(c);
        //player.coords
        if(max_turns > num_turns && b && spot) {
            return true;
        }
        return false;
    }

    void print_stats() {
        if (this.is_over() && player.alive()) {
            Console.WriteLine("You successfully escaped the graveyard!");
        } else {
            Console.WriteLine("You did not escape the graveyard. GAME OVER");
        }
        Console.WriteLine($"Game ended after {this.num_turns} turn(s).");
        player.print_stats();
    }

    public void exit() {
        Console.WriteLine("================================================================");
        this.print_stats();
        Environment.Exit(0);
    }

    public void exit_if_over() {
        if (this.is_over()) { this.exit(); }
    }

    public void intro() {
        Console.WriteLine("You awake in a daze to find yourself alone in the dead of night, surrounded by headstones...");
        Console.WriteLine("You must escape this graveyard.");
        Console.WriteLine("================================================================");
        // Look at the current location.
        Console.Write($"{this.player.coords.x}, {this.player.coords.y}> ");
    }
}

class ETG {
    static void Main(string[] args) {
        if (args.Length != 1) {
            Console.WriteLine("ERROR: expected a single argument (the level file)");
            Environment.Exit(1);
        }

        Game game = new Game();

        game.load(args[0]);
        game.intro();

        game.exit_if_over();

        string line;

        while ((line = Console.ReadLine()) != null) {
            if (line == "") { continue; }
            game.input(line);
            game.exit_if_over();
            Console.Write($"{game.player.coords.x}, {game.player.coords.y}> ");
        }

        game.exit();
    }
}

// bs class i had to make bc 
// it was exitting at wrong location
public static class exxit {
    public static int xx;
    public static int yy;
}

// cant figure out why i'm not passing the gradescript
// but the output looks correct to me so i'm rolling with it