using System;
using System.IO;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
namespace FilterStatistic
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			StreamReader sr = new StreamReader("/Users/apple/Project/benchmarks/FiltersList");
			Dictionary<string, int> dict = new Dictionary<string, int> ();
			HashSet<string> NameSet = new HashSet<string>();
			int cnt = 0;

			foreach (string s in sr.ReadToEnd().Split('\n')) {
				string[] x = s.Split ('\t');
				if (x.Length != 5)
					continue;
				if (x [0] != "0")
					continue;

				cnt++;
				string Name = x [1].Substring (0, x[1].LastIndexOf ("_"));
				//int Push = int.Parse(x[2]);
				int Pop = int.Parse (x [3]);
				int Peek = int.Parse (x [4]);

				int value = Math.Max (Pop, Peek);
				if (dict.ContainsKey (Name)) {
					dict [Name] = Math.Max (value, dict [Name]);
					NameSet.Add (Name);
				}
				else
					dict [Name] = value;
			}

			Console.WriteLine ("Dynamic Filter Count : " + cnt);
			Console.WriteLine ("Static Filter Count : " + dict.Count);

			cnt = 0;
			foreach (var x in dict)
				if (x.Value > 1)
					cnt++;
			Console.WriteLine ("Filter With Push Multi Elements : " + cnt);
			Console.WriteLine ("Count with different Pushes : " + NameSet.Count);

			List<KeyValuePair<string, int>> Filters = dict.ToList ();
			Filters.Sort ((a, b) => {
				return b.Value.CompareTo(a.Value); });
			foreach(var x in Filters)
				Console.WriteLine (x.Key + " " + x.Value);
		}
	}
}
