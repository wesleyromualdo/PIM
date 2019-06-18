<?php
class QGeraScript{
	private $g;
	private $q;
	private $p;
	private $s;
	private $it;
	public $arvore;
	private $tipoArvore;
	private $img;
	private $questionario;
    private $perid;
    private $qrpid;
    private $db;
    static $i;

    /*
	 * Monta a Tela de Geração de Script do Questionário
	 *
	 * include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/QGeraScript.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 *
	 * $obGera = new QGeraScript();
	 * echo $obGera->montaArvoreQ();
	 *
	 * Desenvolvedor: Victor Benzi
	 *
	 */

	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->t  = new Tabela();
		$this->gc = new Grupocoluna();
		$this->gl = new Grupolinha();
		$this->c  = new Coluna();
		$this->l  = new Linha();
		$this->it = new QItempergunta();
		$this->r  = new QResposta();

		$this->perid = $param['perid'];
		$this->queid = array( "queid" => $_REQUEST['queid'] );

		global $db;
		$this->db = $db;
	}

	function montaRelatorio($arEntidade){
		// Formato do $arEntidade => ('entidade', 'qrpid'). EX: array(	0 => ("entidade" => "Acrelandia - AC", "qrpid" => 1234 ),
		//																1 => ("entidade" => "Assis Brasil - AC", "qrpid" => 5678 )	
		//														);

		if( is_array($arEntidade) ){
			foreach( $arEntidade as $entidade ){
				$sql = "SELECT queid FROM questionario.questionario WHERE qrpid = ".$entidade['qrpid'];
				$queid = $this->db->pegaUm($sql);
				
				$obPai = array( 'tipo' => 'queid', 'var' => $queid );
				
				// Busca Grupo Pergunta associadas
				self::montaRelatorioG($obPai);
				
				// Busca Perguntas associadas
				self::montaRelatorioP($obPai);
		
			}
		}
		

		// MOSTRA O RELATORIO
		self::show();
	}

	function montaRelatorioG( $obPai ){

		// Filtra Grupos vinculadas
		$tipo = $obPai['tipo'] == 'grpid' ? 'gru_'.$obPai['tipo'] : $obPai['tipo'];
		$sql = "SELECT grpid FROM questionario.grupopergunta WHERE ".$tipo." = ".$obPai['var'];
		$arGrpid = $this->db->carregarColuna($sql);
		
		foreach ($arGrpid as $grpid){
			
			$obGrupoPergunta = array( 'tipo' => 'grpid', 'var' => $grpid );

			// Busca Grupo Pergunta associadas
			self::montaRelatorioG($obGrupoPergunta);

			// Busca Perguntas associadas
			self::montaRelatorioP($obGrupoPergunta);
		}
		
		//$this->htm = $htm;
	}

	function montaRelatorioP( $obPai ){
		// Filtra perguntas vinculadas
		$sql = "SELECT perid, pertitulo, pertipo FROM questionario.pergunta WHERE ".$obPai['tipo']." = ".$obPai['var'];
		$arPerguntas = $this->db->carregar($sql);

		foreach ($arPerguntas as $pergunta){
			
			$this->html = "";

			// Busca Campo Externo (Tabela)
			self::montaRelatorioExterno( $obPergunta );

			// Busca Itens associados
			self::montaRelatorioI( $obPergunta );
		}
		return $perguntas;
	}

	function montaRelatorioExterno( $obPai ){
		$i = 0;
		$arrObExtPergunta = (array) $this->t->listaObjeto( array( $obPai ) );

		foreach( $arrObExtPergunta as $obExtPergunta ){
			$tblpercent 	  = $obExtPergunta->tblpercent ? $obExtPergunta->tblpercent : 'NULL';
			$tbltotalizadorTx = $obExtPergunta->tbltotalizador ? ', tbltotalizador' : '';
			$tbltotalizador   = $obExtPergunta->tbltotalizador ? ", '".($obExtPergunta->tbltotalizador)."'" : '';
			$tabelas[] = array(
				"SQL" => "INSERT INTO questionario.tabela(perid, tbldescricao, tbllabelcolunas, tbllabellinhas, tbltipo, tblpercent ".$tbltotalizadorTx.") VALUES ( resultPai, '".($obExtPergunta->tbldescricao)."', '".($obExtPergunta->tbllabelcolunas)."', '".($obExtPergunta->tbllabellinhas)."', '".($obExtPergunta->tbltipo)."', ".$tblpercent." ".$tbltotalizador.") RETURNING tblid",
				"tipo" => "tabela",
				"var"  => "tblid",
				"itens" => array());

			// Verifica se tem Grupo de Colunas
			$arrGC = self::montaRelatorioExternoGrupoColuna( $obExtPergunta );
			if( is_array( $arrGC ) ){
				array_push( $tabelas[$i]['itens'], $arrGC );
			}

			//Verifica se tem Grupo de Linhas
			$arrGL = self::montaRelatorioExternoGrupoLinha( $obExtPergunta );
			if( is_array( $arrGL ) ){
				array_push( $tabelas[$i]['itens'], $arrGL );
			}

			$i++;
		}

		return $tabelas;
	}

	function montaRelatorioExternoGrupoColuna( $obPai ){

		$gru = '';
		$i = 0;
		if( simec_get_class($obPai) == 'Grupocoluna' ){
			$gru = 'gru_';
		}

		$arrObExtGrupoColuna = (array) $this->gc->listaObjeto( array( $obPai ) );
		//ver($arrObExtGrupoColuna);

		foreach( $arrObExtGrupoColuna as $obExtGrupoColuna ){
			$grupocoluna[] = array(
				"SQL" => "INSERT INTO questionario.grupocoluna(".$gru."varPai, grcdsc) VALUES ( resultPai, '".($obExtGrupoColuna->grcdsc)."') RETURNING grcid",
				"tipo" => "grupocoluna",
				"var"  => "grcid",
				"itens" => array());

			// Verifica se tem Grupo de Colunas filho dele
			$arrGC = self::montaRelatorioExternoGrupoColuna( $obExtGrupoColuna );
			if( is_array( $arrGC )){
				array_push( $grupocoluna[$i]['itens'], $arrGC );
			}

			// Verifica se tem Grupo de Linhas
			$arrC = self::montaRelatorioExternoColuna( $obExtGrupoColuna );
			if( is_array( $arrC ) ){
				array_push( $grupocoluna[$i]['itens'], $arrC );
			}

			$i++;
		}

		return $grupocoluna;
	}

	function montaRelatorioExternoColuna( $obPai ){

		$arrObExtColuna = (array) $this->c->listaObjeto( array( $obPai ) );

		foreach( $arrObExtColuna as $obExtColuna ){
			$coltipoTx 		    = $obExtColuna->coltipo ? ', coltipo' : '';
			$coltipo 		    = $obExtColuna->coltipo ? ", '".($obExtColuna->coltipo)."'" : '';
			$collegendaTx 	    = $obExtColuna->collegenda ? ', collegenda' : '';
			$collegenda 	    = $obExtColuna->collegenda ? ", '".($obExtColuna->collegenda)."'" : '';
			$colmascaraTx 	    = $obExtColuna->colmascara ? ', colmascara' : '';
			$colmascara 	    = $obExtColuna->colmascara ? ", '".($obExtColuna->colmascara)."'" : '';
			$coltamanhomaximoTx = $obExtColuna->coltamanhomaximo ? ', coltamanhomaximo' : '';
			$coltamanhomaximo   = $obExtColuna->coltamanhomaximo ? ", ".$obExtColuna->coltamanhomaximo."" : '';
			$coluna[] = array(
				"SQL" => "INSERT INTO questionario.coluna(grcid, itmid ".$coltipoTx.$collegendaTx.$colmascaraTx.$coltamanhomaximoTx.") VALUES ( resultPai, varItem ".$coltipo.$collegenda.$colmascara.$coltamanhomaximo." ) RETURNING grcid",
				"tipo" => "coluna",
				"itmdsc"  => ($this->c->pegaItem($obExtColuna->itmid)),
				"var"  => "colid",
				"itens" => array());
		}

		return $coluna;
	}

	function montaRelatorioExternoGrupoLinha( $obPai ){

		$gru = '';
		$i = 0;
		if( simec_get_class($obPai) == 'Grupolinha' ){
			$gru = 'gru_';
		}

		$arrObExtGrupoLinha = (array) $this->gl->listaObjeto( array( $obPai ) );
		//ver($arrObExtGrupoColuna);

		foreach( $arrObExtGrupoLinha as $obExtGrupoLinha ){
			$grupolinha[] = array(
				"SQL" => "INSERT INTO questionario.grupolinha(".$gru."varPai, grldsc) VALUES ( resultPai, '".($obExtGrupoLinha->grldsc)."') RETURNING grlid",
				"tipo" => "grupolinha",
				"var"  => "grlid",
				"itens" => array());

			// Verifica se tem Grupo de Colunas filho dele
			$arrGL = self::montaRelatorioExternoGrupoLinha( $obExtGrupoLinha );
			if( is_array( $arrGL )){
				array_push( $grupolinha[$i]['itens'], $arrGL );
			}

			// Verifica se tem Grupo de Linhas
			$arrC = self::montaRelatorioExternoLinha( $obExtGrupoLinha );
			if( is_array( $arrC ) ){
				array_push( $grupolinha[$i]['itens'], $arrC );
			}

			$i++;
		}

		return $grupolinha;
	}

	function montaRelatorioExternoLinha( $obPai ){

		$arrObExtLinha = (array) $this->l->listaObjeto( array( $obPai ) );

		foreach( $arrObExtLinha as $obExtLinha ){
			$lindscTx = $obExtLinha->lindsc ? ', lindsc' : '';
			$lindsc = $obExtLinha->lindsc ? ", '".($obExtLinha->lindsc)."'" : '';
			$linha[] = array(
				"SQL" => "INSERT INTO questionario.linha(grlid, itmid ".$lindscTx.") VALUES ( resultPai, varItem ".$lindsc." ) RETURNING linid",
				"tipo" => "linha",
				"itmdsc"  => ($this->c->pegaItem($obExtLinha->itmid)),
				"var"  => "linid",
				"itens" => array());
		}

		return $linha;
	}

	function montaRelatorioI( $obPai ){

		$where[0] = $obPai;

		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		$i = 0;
		foreach ($arrObItPergunta as $obItPergunta){
			$itens[] = array(
				"SQL" => "INSERT INTO questionario.itempergunta(perid, itptitulo) VALUES ( resultPai, '".($obItPergunta->itptitulo)."') RETURNING itpid",
				"tipo" => "item",
				"var"  => "itpid",
				"itens" => array());

			// Busca Perguntas associadas
			$arrP = self::montaRelatorioP($obItPergunta);
			if( is_array( $arrP )){
				array_push( $itens[$i]['itens'], $arrP );
			}

			// Busca Grupo Pergunta associadas
			$arrG = self::montaRelatorioG($obItPergunta);
			if( is_array( $arrG )){
				array_push( $itens[$i]['itens'], $arrG );
			}
			$i++;
		}
		return $itens;
	}
	
	//Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
	function show(){
		echo $this->htm;
	}

}
?>