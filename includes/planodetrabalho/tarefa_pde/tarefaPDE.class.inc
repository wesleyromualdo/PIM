<?php
	class TarefaPDE extends Tarefa
	{
		/**
		 * Nome da tabela do banco onde ficam armazenadas as tarefas
		 *
		 */
		public $TABLE_NAME = 'pde.atividade';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 				= 'atvid';
		
		/**
		 * @var integer
		 * Campo para a chave do id do dono da tarefa
		 */		
		public $ROW_NAME_OWNER_ID 	= '';
		
		/**
		 * Chave estrageira do id da tarefa predecessora direta
		 * 
		 * A predecessora direta é a tarefa que esta depende estar concluida
		 * para ser iniciada. Cada tarefa só tem uma predecessora direta mas
		 * várias tarefas podem ser predecessidas diretamente pela mesma 
		 * tarefa.
		 */
		public $ROW_NAME_PREDECESSOR_ID 	= '';

		/**
		 * Chave estrangeira do id da tarefa que contém esta tarefa
		 * 
		 * As tarefas podem conter tarefas. As tarefas que não são contidas
		 * por nenhum outra tarefa possuem este campo nulo enquanto as que
		 * pertencem a uma tarefa possuem o id da tarefa que a contem neste
		 * campo.
		 *
		 */
		public $ROW_NAME_CONTAINER_ID 	= 'atvpai';
		
		/**
		 * Chave estrangeira do id do projeto de que estas tarefas pertencem.
		 * 
		 * As tarefas pertecem a um projeto então todas as tarefas devem
		 * ter o id do projeto a quem pertencem. As tarefas não devem exceder
		 * os limites das datas do projeto. Caso alguma tente fazer isto é 
		 * checado se o projeto possui flexibilidade de data, se isto for 
		 * verdade o mesmo é alterado para a tarefa permanecer dentro dos 
		 * seus limites, se não é retornado um erro.
		 *
		 */
		public $ROW_NAME_PROJECT_ID 		= 'atvid';
		
		/**
		 * Coluna de texto que contém o nome da tarefa
		 */
		public $ROW_NAME_STR_NAME 		= 'atvdescricao';
		
		/**
		 * Coluna de texto que contém a descrição da tarefa
		 *
		 */
		public $ROW_NAME_STR_DESC 		= 'atvdetalhamento';
		
		/**
		 * Coluna numérica inteira que contém um código único, no escopo
		 * do projeto, de cada tarefa.
		 *
		 */
		public $ROW_NAME_UNIQUE_CODE 		= '';
		
		/**
		 * Coluna de data que contém a data de início da tarefa
		 */
		public $ROW_NAME_DATE_START 		= 'atvdatainicio';
		
		/**
		 * Coluna de data que contém a data de término da tarefa
		 */
		public $ROW_NAME_DATE_END 		= 'atvdatafim';
		
		/**
		 * Coluna booleana que informa se a tarefa possui a data
		 * fechada ou não.
		 * 
		 * Caso a data seja fechada não se pode altera-la por efeito
		 * colateral das tarefas relacionadas, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		public $ROW_NAME_BOOL_CLOSED_DATE	= '';

		/**
		 * Coluna inteira para o controle do aviso de mensagens
		 * quando a data de termino se aproxima
		 */
		public $ROW_NAME_INT_ALERT_MISSING_DAYS = '';

		/**
		 * Chave estrangeira para a coluna do Produto 
		 */
		public $ROW_NAME_PRODUCT_ID	= '';

		/**
		 * Chave estrangeira para a coluna da unidade de medida do
		 * progresso da tarefa 
		 */
		public $ROW_NAME_MEAUSE_UNIT_ID	= '';
		
		/**
		 * Campo de valor do controle da meta da Tarefa
		 */
		public $ROW_NAME_DBL_AIM	= '';

		/**
		 * Coluna booleana que informa se a tarefa esta em uso
		 * ou nao. 
		 *
		 */
		public $ROW_NAME_BOOL_ACTIVE_STATUS	= 'atvstatus';

		/**
		 * Posicao da ordenacao das tarefas filhas
		 * de um mesmo nó
		 */
		public $ROW_NAME_INT_POSITION = '';
		
		public function getProjeto()
		{
			if( ( $this->objProjeto === NULL ) && ( $this->intProjetoId != NULL ) )
			{
				$this->objProjeto = ProjetoPDE::getProjetoPeloId( $this->intProjetoId );
			}
			return $this->objProjeto;
		}
		
		public function criaTarefa()
		{
			return new TarefaPDE();
		}
		
	}
?>