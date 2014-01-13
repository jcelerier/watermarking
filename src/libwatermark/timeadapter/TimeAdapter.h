#pragma once
#include <functional>
#include <vector>

/**
 * @brief The TimeAdapter class
 *
 * Classe de base pour activer les algorithmes à un certain moment.
 * La base de temps est le buffer et non l'échantillon !
 *
 * Il y a deux choses qui se passent en parallèle.
 * - D'une part, des évènements sont déclenchés lorsque les conditions startCheck et stopCheck sont vérifiées.
 * - D'autre part, entre startCheck et stopCheck l'état "running" est à true, et à false sinon.
 *
 */
class TimeAdapter
{
	protected:
		virtual bool startCheck() = 0;
		virtual bool stopCheck() = 0;

	public:
		virtual ~TimeAdapter() = default;

		/**
		 * @brief callHandlers Appelle les handlers
		 * @param h Un tableau de handlers
		 */
		void callHandlers(std::vector<std::function<void(void)>> h)
		{
			for(auto& fun : h)
			{
				fun();
			}
		}

		/**
		 * @brief addStartHandler
		 * @param f Appel de fonction
		 *
		 * Le prototype de la fonction doit être "void f();" ou "void MaClasse::f();" avec un bind.
		 * Les fonctions sont appelées à chaque start, dans n'importe quel ordre.
		 */
		void addStartHandler(std::function<void(void)> f)
		{
			startHandlers.push_back(f);
		}

		/**
		 * @brief addStopHandler
		 * @param f Appel de fonction
		 *
		 * Le prototype de la fonction doit être "void f();" ou "void MaClasse::f();" avec un bind.
		 * Les fonctions sont appelées à chaque stop, dans n'importe quel ordre.
		 */
		void addStopHandler(std::function<void(void)> f)
		{
			stopHandlers.push_back(f);
		}

		/**
		 * @brief perform
		 *
		 * Doit être appelé à chaque passage de buffer, pour réaliser les tests nécessaires.
		 */
		void perform()
		{
			if(startCheck())
			{
				running = true;
				callHandlers(startHandlers);
			}

			if(stopCheck())
			{
				running = false;
				callHandlers(stopHandlers);
			}

			++count;
		}

		/**
		 * @brief isRunning
		 * @return Vrai si on est entre start et stop ([start; stop[), faux sinon.
		 */
		bool isRunning()
		{
			return running;
		}

	protected:
		long unsigned int count = 0;
		bool running = true;

	private:
		std::vector<std::function<void(void)>> startHandlers = {};
		std::vector<std::function<void(void)>> stopHandlers = {};
};

using TimeAdapter_p = std::shared_ptr<TimeAdapter>;
