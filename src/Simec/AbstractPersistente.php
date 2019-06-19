<?php

namespace Simec;

function buffer_persistente($buffer) {
//	return $buffer;
	$arDebug = explode("\n", $buffer);
	$res = "";
	foreach ($arDebug as $chamada => $linha) {
		if (preg_match('/^#(\d*)(.*)\(/', $linha, $f)) {
			$n = isset($f[1]) ? $f[1] : '?????';
			$funcao = isset($f[2]) ? $f[2] : '?????';
			$pos = strpos($funcao, '(');
			if ($pos !== false) {
				$funcao = substr($funcao, 0, $pos);
			}
		}
		if (preg_match('/ called at \[(.*):(\d*)\]\s*$/', $linha, $l)) {
			$linha = isset($l[1]) ? $l[1] : '????';
			$nrLinha = isset($l[2]) ? $l[2] : '???';
			$res.= "#{$n}<font class='metodo'>{$funcao}</font> [<font class='variavel'>{$linha}</font>:<font class='numero'>{$nrLinha}</font>]<br/>";
		}
	}
	return $res;
}

/**
 * Classe de representação de uma camada de persistencia com Banco de Dados
 * @package FrameCalixto
 * @subpackage Persistente
 */
abstract class persistente extends objeto {

	/**
	 * Classe de internacionalização para documentação das tabelas
	 * @var internacionalizacao 
	 */
	protected static $inter = false;

	/**
	 * @var boolean variável de debugger
	 */
	protected static $imprimirComandos = false;
	protected static $pilhaDeChamadas = false;
	protected static $arquivoLog = false;
	protected $schema = null;

	/**
	 * @var array array com a estrutura dos objetos persistentes
	 * criado para a execução de cache
	 */
	private static $estrutura;

	/**
	 * @var conexao objeto de conexão com o banco de dados
	 */
	public $conexao;

	/**
	 * Metodo construtor
	 * @param conexao (opcional) conexão com o banco de dados
	 * @param string (opcional) nome do arquivo de configuração da persistente
	 */
	public function __construct(conexao $conexao, $arquivoXML = null) {
		$this->conexao = $conexao;
	}

	/**
	 * Retorna se a persistente está imprimindo os comandos de execução
	 * @return boolean 
	 */
	public static function estaImprimindoComandos() {
		return (boolean) self::$imprimirComandos;
	}

	/**
	 * Configura a persistente para imprimir os comandos de execução
	 * @param boolean $valor
	 * @param boolean $pilhaDeChamadas
	 */
	public static function imprimirComandos($valor, $pilhaDeChamadas = true) {
		$ar = debug_backtrace();
		echo "<link rel='stylesheet' href='.sistema/debug.css' /><div class='debug'>persistente configurando impressão por :{$ar[0]['file']} na linha:{$ar[0]['line']}</div>";
		self::$imprimirComandos = (boolean) $valor;
		self::$pilhaDeChamadas = (boolean) $pilhaDeChamadas;
	}

	/**
	 * Método de sobrecarga para printar a classe
	 * @return string texto de saída da classe
	 */
	public function __toString() {
		debug2($this);
		echo '<pre>';
		echo $this->comandoCriacaoCompleto();
		echo '</pre>';
		debug2($this->pegarEstrutura());
		return '';
	}

	/**
	 * @return internacionalizacao
	 */
	public function internacionalizacao() {
		try {
			if (isset(self::$inter[definicaoEntidade::internacionalizacao($this)])) {
				return self::$inter[definicaoEntidade::internacionalizacao($this)];
			} else {
				$internacionalizacao = definicaoEntidade::internacionalizacao($this);
				return self::$inter[definicaoEntidade::internacionalizacao($this)] = new $internacionalizacao();
			}
		} catch (\Exception $e) {
			return false;
		}
	}

	/**
	 * Metodo criado para especificar a estrutura da persistente
	 * @param string nome da persistente
	 */
	public function pegarEstrutura($nomePersistente = null) {
		if ($nomePersistente) {
			if (isset(persistente::$estrutura[$nomePersistente])) {
				return persistente::$estrutura[$nomePersistente];
			} else {
				throw new \Simec\Excecao\Banco('Não foi possível acessar a estrutura da persistente, não existe instancia ativa de ' . $nomePersistente);
			}
		}
		if (!isset(persistente::$estrutura[get_class($this)])) {
			$arquivoXML = definicaoArquivo::pegarXmlEntidade($this, null);
			switch (true) {
				case!($arquivoXML):
					break;
				case!(is_file($arquivoXML)):
					throw new erroInclusao("Arquivo [$arquivoXML] inexistente!");
					break;
				case!(is_readable($arquivoXML)):
					throw new erroInclusao("Arquivo [$arquivoXML] sem permissão de leitura!");
					break;
				default:
					$xml = simplexml_load_file($arquivoXML);
					$estrutura['nomeSchema'] = strval($xml['schema']);
					$estrutura['nomeTabela'] = strval($xml['nomeBanco']);
					$estrutura['nomeSequencia'] = strval($xml['nomeSequencia']) ? strval($xml['nomeSequencia']) : "sq_{$estrutura['nomeTabela']}";
					foreach ($xml->propriedades->propriedade as $campo) {
						$nomeCampo = strtolower(strval($campo->banco['nome']));
						if (isset($campo['indicePrimario']) && strtolower(strval($campo['indicePrimario'])) == 'sim') {
							$estrutura['chavePrimaria'] = $nomeCampo;
						}
						$estrutura['campo'][$nomeCampo]['propriedade'] = strval($campo['id']);
						$estrutura['campo'][$nomeCampo]['nome'] = $nomeCampo;
						$estrutura['campo'][$nomeCampo]['tipo'] = strtolower(strval($campo['tipo']));
						$estrutura['campo'][$nomeCampo]['tamanho'] = strval($campo['tamanho']);
						$estrutura['campo'][$nomeCampo]['obrigatorio'] = (strtolower(strval($campo['obrigatorio'])) == 'sim') ? 'sim' : 'nao';
						if ($estrutura['campo'][$nomeCampo]['tipo'] == 'texto') {
							$estrutura['campo'][$nomeCampo]['operadorDeBusca'] = isset($campo->banco['operadorDeBusca']) ? strtolower(strval($campo->banco['operadorDeBusca'])) : operador::como;
						} else {
							$estrutura['campo'][$nomeCampo]['operadorDeBusca'] = isset($campo->banco['operadorDeBusca']) ? strtolower(strval($campo->banco['operadorDeBusca'])) : operador::igual;
						}
						if (isset($campo->banco->chaveEstrangeira)) {
							$estrutura['campo'][$nomeCampo]['chaveEstrangeira']['tabela'] = strval($campo->banco->chaveEstrangeira['tabela']);
							$estrutura['campo'][$nomeCampo]['chaveEstrangeira']['campo'] = strval($campo->banco->chaveEstrangeira['campo']);
						}
						if (isset($campo->dominio->opcao)) {
							foreach ($campo->dominio->opcao as $opcao) {
								$estrutura['campo'][$nomeCampo]['valoresPossiveis'][] = strval($opcao['id']);
							}
						}
						if (isset($campo->banco['ordem'])) {
							if (isset($campo->banco['tipoOrdem']) && $campo->banco['tipoOrdem'] == 'inversa') {
								$estrutura['ordem'][strval($campo->banco['ordem'])] = $nomeCampo . ' desc';
							} else {
								$estrutura['ordem'][strval($campo->banco['ordem'])] = $nomeCampo;
							}
						}
					}
					break;
			}
			if (isset($estrutura['ordem']))
				ksort($estrutura['ordem']);
			persistente::$estrutura[get_class($this)] = $estrutura;
		}
		return persistente::$estrutura[get_class($this)];
	}

	/**
	 * Configura o arquivo de log da persistente
	 * @param string $arquivo caminho e nome do arquivo de log
	 */
	public static function arquivoLog($arquivo) {
		self::$arquivoLog = $arquivo;
	}

	/**
	 * Executa um comando SQL no banco de dados.(necessita de controle de transação)
	 * @param string comando SQL para a execução
	 * @return integer número de linhas afetadas
	 */
	public function executarComando($comando = null) {
		if (self::$imprimirComandos) {
			if (!is_integer(self::$imprimirComandos)) {
				self::$imprimirComandos = 1;
			} else {
				self::$imprimirComandos++;
			}
			$nrComando = self::$imprimirComandos;
			$sqlClass = $nrComando % 2 ? 'sql1' : 'sql2';
			echo "<br/><table class='{$sqlClass} objeto'>";
			echo "<tr class='linha'>";
			if (self::$pilhaDeChamadas) {
				echo "<td class='numero' rowspan='2'>{$nrComando}</td>";
				echo "<td class='string'><pre>";
				ob_start('buffer_persistente');
				debug_print_backtrace();
				ob_end_flush();
				echo "</pre></td></tr><tr>";
				echo "<td class='string'><pre>{$comando}</pre></td>";
				echo "</tr></table>";
			} else {
				echo "<td class='numero'>{$nrComando}</td>";
				echo "<td class='string'><pre>{$comando}</pre></td>";
				echo "</tr></table>";
			}
		}
		if (persistente::$arquivoLog) {
			static $sqlLog = 0;
			$sqlLog++;
			file_put_contents(persistente::$arquivoLog, "
--------------------------------------------------------------
-- Comando {$sqlLog}
--------------------------------------------------------------
{$comando}
--------------------------------------------------------------


				", FILE_APPEND);
		}
		return $this->conexao->executarComando($comando);
	}

	/**
	 * Monta o mapeamento de tipo de dados do banco
	 * @return array mapeamento
	 */
	public abstract function mapeamento();

	//**************************************************************************
	//**************************************************************************
	// 							COMANDOS DML
	//**************************************************************************
	//**************************************************************************
	/**
	 * Método de conversão de tipo de dado
	 * @param mixed dado a ser convertido
	 * @param array campo referente
	 */
	public function converterDado($valor, $campo = null) {
		if ($campo) {
			switch (strtolower($campo['tipo'])) {
				case 'data e hora':
				case 'datahora':
					return new TDataHora($valor);
					break;
				case 'data':
					return new TData($valor);
					break;
				case 'tnumerico':
					return new TNumerico((float) $valor);
					break;
				case 'tmoeda':
					return new TMoeda((float) $valor);
					break;
				case 'ttelefone':
					return new TTelefone($valor);
					break;
				case 'tcep':
					return new TCep($valor);
					break;
				case 'tcpf':
					return new TCpf($valor);
					break;
				case 'tcnpj':
					return new TCnpj($valor);
					break;
				default:
					return $valor;
			}
		} else {
			switch (true) {
				case($valor instanceof TNumerico):
				case($valor instanceof TDocumentoPessoal):
					return $valor->pegarNumero();
					break;
				case(is_object($valor)):
					return $valor->__toString();
					break;
				default:
					return $valor;
			}
		}
	}

	/**
	 * Método de formatação dos registros para os dados de outras persistentes
	 * @param array $estrutura
	 * @param array $tupla
	 */
	public function formatarRegistro($estrutura, &$tupla) {
		foreach ($estrutura['campo'] as $campo => $atributos) {
			if (isset($tupla[$campo])) {
				$tupla[$campo] = $this->converterDado($tupla[$campo], $atributos);
			}
		}
	}

	/**
	 * Retorna o registro corrente na conexão com o banco.(necessita de controle de transação)
	 * @return array registro corrente
	 */
	public function pegarRegistro() {
		if (!is_subclass_of($this->conexao, 'conexao'))
			throw new \Simec\Excecao\Banco('Utilização incorreta da persistente! Possívelmente você efetuou uma chamada do método ' . get_class($this) . '::pegarRegistro sem controle de conexão!');
		$tupla = $this->conexao->pegarRegistro();
		$estrutura = $this->pegarEstrutura();
		$this->formatarRegistro($estrutura, $tupla);
		return $tupla;
	}

	/**
	 * Retorna a seleção de registros da conexão com o banco
	 * @param string $comando SQL para a execução
	 * @return array seleção de registros
	 */
	public function pegarSelecao($comando = null) {
		$this->executarComando($comando);
		while ($arTupla = $this->pegarRegistro()) {
			$recordSet[] = $arTupla;
		}
		$retorno = isset($recordSet) ? $recordSet : false;
		return $retorno;
	}

	public function criarSchema() {
		$res = $this->pegarSelecao('SELECT 1 as schema FROM pg_namespace WHERE nspname = \'' . definicaoBanco::pegarSchema() . '\'');
		if (!$res)
			$this->executarComando('create schema ' . definicaoBanco::pegarSchema());
	}

	/**
	 * Nome do schema utilizado na persistente
	 * @param boolean $ponto setando para true já vem com o ponto para ligar na tabela
	 * @return string
	 */
	public function pegarNomeSchema($ponto = true) {
		$estrutura = $this->pegarEstrutura();
		switch (true) {
			case $this->schema :
				$schema = $this->schema;
				break;
			case $estrutura['nomeSchema'] :
				$schema = $estrutura['nomeSchema'];
				break;
			default:
				$schema = definicaoBanco::pegarSchema();
		}
		if (!$schema)
			return '';
		return $schema . ($ponto ? '.' : '');
	}

	/**
	 * Retorna o nome da tabela utilizada pela persistente
	 * @param boolean verificador se retorna com o nome do schema
	 * @return string Nome da tabela
	 */
	public function pegarNomeTabela($comSchema = true) {
		$estrutura = $this->pegarEstrutura();
		if (strpos($estrutura['nomeTabela'], '.') !== false)
			return $estrutura['nomeTabela'];
		if (!$comSchema)
			return strtolower($estrutura['nomeTabela']);
		return strtolower($this->pegarNomeSchema() ? $this->pegarNomeSchema() . $estrutura['nomeTabela'] : $estrutura['nomeTabela']);
	}

	/**
	 * Retorna o nome da sequencia de banco utilizada pela persistente
	 * @return string Nome da Sequencia
	 */
	public function pegarNomeSequencia($comSchema = true) {
		$estrutura = $this->pegarEstrutura();
		if (strpos($estrutura['nomeSequencia'], '.') !== false)
			return $estrutura['nomeSequencia'];
		if (!$comSchema)
			return strtolower($estrutura['nomeSequencia']);
		return strtolower($this->pegarNomeSchema() ? $this->pegarNomeSchema() . $estrutura['nomeSequencia'] : $estrutura['nomeSequencia']);
	}

	/**
	 * Gera o comando SQL de leitura de todos os registros
	 * @return string comando SQL de leitura de um registro
	 */
	public function gerarComandoLerTodos() {
		$estrutura = $this->pegarEstrutura();
		return "select * from {$this->pegarNomeTabela()} ";
	}

	/**
	 * Executa o comando de leitura de todos os registros
	 * @param pagina $pagina
	 * @return array seleção de registros
	 */
	public function lerTodos(pagina $pagina = null) {
		if (!$pagina)
			return $this->pegarSelecao($this->gerarComandoLerTodos() . $this->gerarClausulaOrdem());
		return $this->lerPaginado($pagina, $this->gerarComandoLerTodos() . $this->gerarClausulaOrdem());
	}

	/**
	 * Método de verificação da existência de uma tabela no banco de dados
	 * @param string $tabela
	 * @return boolean
	 */
	public function existeTabela($tabela = false) {
		try {
			$tabela = $tabela ? $tabela : $this->pegarNomeTabela();
			$this->executarComando("select count(*) from {$tabela}");
			return true;
		} catch (\Exception $e) {
			return false;
		}
	}

	/**
	 *
	 * @param string $valor valor sem tratamento de sql injection
	 * @return string tratada
	 */
	public function tratarInjection($valor) {
		return str_replace("'", "''", $valor);
	}

	/**
	 * Método que cria um item de filtragem na cláusula where
	 * @param operador $operador operador do item de filtro
	 * @param string $campo campo  do item de filtro
	 * @param string $tipo tipo  do item de filtro
	 * @return string
	 */
	public function gerarItemDeFiltro(operador $operador, $campo, $tipo) {
		if ((!$operador->pegarValor()) && ($operador->pegarOperador() != operador::eNulo) && ($operador->pegarOperador() != operador::naoENulo))
			return null;
		switch ($operador->pegarOperador()) {
			case(operador::eNulo) : return "{$campo} is null {$operador->pegarRestricao()} ";
			case(operador::naoENulo) : return "{$campo} is not null {$operador->pegarRestricao()} ";
			case(operador::entre):
				$val = $operador->pegarValor();
				foreach ($operador->pegarValor() as $i => $parte) {
					$val[$i] = $this->tratarInjection($parte);
				}
				if ($tipo == 'numero') {
					return "  ({$campo} between {$val['valor1']} and {$val['valor2']} ) {$operador->pegarRestricao()} ";
				} else {
					return "  ({$campo} between '{$val['valor1']}' and '{$val['valor2']}' ) {$operador->pegarRestricao()} ";
				}
				break;
			case(operador::dominio):
				$val = array();
				foreach ($operador->pegarValor() as $i => $parte) {
					$val[$i] = $this->tratarInjection($parte);
				}
				if ($tipo == 'numero') {
					return " {$campo} in( '" . implode("','", $val) . "' ) {$operador->pegarRestricao()} ";
				} else {
					return " {$campo} in( " . implode(",", $val) . " ) {$operador->pegarRestricao()} ";
				}
				break;
			case(operador::diferente) : $comando = " %s <> '%s' %s ";
				break;
			case(operador::iniciandoComo) : $comando = " upper(%s) like upper('%%%s') %s ";
				break;
			case(operador::finalizandoComo) : $comando = " upper(%s) like upper('%s%%') %s ";
				break;
			case(operador::como) : $comando = " upper(%s) like upper('%%%s%%') %s ";
				break;
			case(operador::generico) : $comando = " upper(%s) like upper('%%%s%%') %s ";
				break;
			case(operador::igual) : $comando = " %s = '%s' %s ";
				break;
			case(operador::maiorOuIgual) : $comando = " %s >= '%s' %s ";
				break;
			case(operador::maiorQue) : $comando = " %s > '%s' %s ";
				break;
			case(operador::menorQue) : $comando = " %s < '%s' %s ";
				break;
			case(operador::menorOuIgual) : $comando = " %s <= '%s' %s ";
				break;
		}
		if ($tipo == 'numero')
			str_replace("'", '', $comando);
		return sprintf($comando, $campo, $this->tratarInjection($operador->pegarValor()), $operador->pegarRestricao());
	}

	/**
	 * Gera a cláusula de filtro de leitura
	 * @param array $filtro
	 * @param boolean $nomeDaClausula
	 * @return string
	 */
	public function gerarClausulaDeFiltro($filtro, $nomeDaClausula = true) {
		if (!$filtro)
			return '';
		$comando = '';
		$estrutura = $this->pegarEstrutura();
		if (is_array($filtro)) {
			$arItens = array();
			foreach ($filtro as $campo => $valor) {
				if ($valor instanceof operador) {
					$arItens[][$campo] = $valor;
				} else {
					if (isset($estrutura['campo'][$campo])) {
						$operador = new operador();
						$operador->passarOperador($estrutura['campo'][$campo]['operadorDeBusca']);
						$operador->passarRestricao(operador::restricaoE);
						$operador->passarValor($valor);
						$arItens[][$campo] = $operador;
					}
				}
			}
			$filtro = new colecaoPadraoFiltro($arItens);
		}
		foreach ($filtro->itens as $item) {
			list($campo, $operador) = each($item);
			$operador->passarValor($this->converterDado($operador->pegarValor()));
			if (isset($estrutura['campo'][$campo]))
				$comando.=$this->gerarItemDeFiltro($operador, $campo, $estrutura['campo'][$campo]['tipo']);
		}
		if ($comando) {
			$comando = substr($comando, 0, -5);
			if ($nomeDaClausula)
				$comando = ' where ' . $comando;
		}
		return $comando;
	}

	/**
	 * Método que manipula cada item da cláusula de filtro
	 * @param string $operacao referência utilizada na cláusula de filtro
	 * @param array $campo
	 * @param operador $operador
	 * @param mixed $valor
	 * @param mixed $dominio
	 */
	public function manipularItemDeFiltro(&$operacao, $campo, operador $operador, $valor, $dominio) {
		
	}

	public function gerarClausulaOrdem() {
		$estrutura = $this->pegarEstrutura();
		return (isset($estrutura['ordem'])) ? ' order by ' . implode(',', $estrutura['ordem']) : '';
	}

	/**
	 * Gera o comando SQL de leitura dos registros pesquisados
	 * @return string comando SQL de leitura de um registro
	 */
	public function gerarComandoPesquisar($filtro) {
		$estrutura = $this->pegarEstrutura();
		$comando = $this->gerarComandoLerTodos();
		$tamanhoComando = strlen($comando);
		$comando .= $this->gerarClausulaDeFiltro($filtro);
		if ($tamanhoComando != strlen($comando)) {
			$comando = $comando . $this->gerarClausulaOrdem();
		} else {
			$comando = $this->gerarComandoLerTodos();
		}
		return $comando;
	}

	/**
	 * Retorna a quantidade de objetos que o metodo pesquisar irá retornar
	 * @param filtro dados de pesquisa (não obrigatorio)
	 * @return int
	 */
	public function totalDePesquisar($filtro = null) {
		$total = $this->pegarSelecao("select count(*) as quantidade from ({$this->gerarComandoPesquisar($filtro)}) selecao");
		if (isset($total[0]['quantidade'])) {
			return (integer) $total[0]['quantidade'];
		} else {
			return false;
		}
	}

	/**
	 * Executa o comando de leitura dos registros pesquisados
	 * @param array dados do filtro
	 * @param pagina pagina referente
	 * @return array seleção de registros
	 */
	public function pesquisar($filtro, pagina $pagina) {
		if (is_subclass_of($filtro, 'filtro')) {
			trigger_error('Para se utilizar um "filtro" deve-se especializar o método "pesquisar" da persistente [' . get_class($this) . ']');
		}
		if ($pagina->pegarTamanhoPagina() == 0) {
			$res = $this->pegarSelecao($this->gerarComandoPesquisar($filtro));
			$pagina->passarTamanhoGeral(count($res));
			return $res;
		} else {
			return $this->lerPaginado($pagina, $this->gerarComandoPesquisar($filtro));
		}
	}

	/**
	 * Executa o comando de leitura de todos os registros
	 * @param pagina pagina referente
	 * @return array seleção de registros
	 */
	public function lerTodosPaginado(pagina $pagina) {
		return $this->lerPaginado($pagina, $this->gerarComandoLerTodos() . $this->gerarClausulaOrdem());
	}

	/**
	 * Executa o comando de leitura dos registros com paginação
	 * @param pagina pagina referente
	 * @param string comando sql para execução
	 * @return array seleção de registros
	 */
	public function lerPaginado(pagina $pagina, $sql) {
		$total = $this->pegarSelecao("select count(*) as \"quantidade\" from ({$sql}) selecao");
		if (isset($total[0]['quantidade'])) {
			$pagina->passarTamanhoGeral((integer) $total[0]['quantidade']);
		}
		return $this->pegarSelecao($this->gerarComandoLerPaginado($pagina, $sql));
	}

	/**
	 * Gera o comando de leitura paginada
	 * @param pagina pagina referente
	 * @param string comando sql para execução
	 * @return string comando SQL de leitura
	 */
	public function gerarComandoLerPaginado(pagina $pagina, $sql) {
		if ($pagina->pegarTamanhoPagina() == 0) {
			return $sql;
		} else {
			return "select * from ({$sql}) selecao limit " . ($pagina->pegarTamanhoPagina()) . " offset " . ($pagina->pegarLinhaInicial() - 1);
		}
	}

	/**
	 * Gera o comando SQL de leitura de um registro
	 * @param string chave única de identificação do registro
	 * @return string comando SQL de leitura de um registro
	 */
	public function gerarComandoLer($chave) {
		$estrutura = $this->pegarEstrutura();
		if ($chave)
			return "select * from {$this->pegarNomeTabela()} where {$estrutura['chavePrimaria']} = '$chave'";
		return "select * from {$this->pegarNomeTabela()} where {$estrutura['chavePrimaria']} is null";
	}

	/**
	 * Executa o comando de leitura de um registro
	 * @param string chave única de identificação do registro
	 * @return array seleção de registro
	 */
	public function ler($valorChave) {
		$arRetorno = $this->pegarSelecao($this->gerarComandoLer($valorChave));
		if (isset($arRetorno[0]))
			return $arRetorno[0];
	}

	abstract function gerarSequencia();

	abstract function pegarUltimaSequencia();

	/**
	 * Gera o comando de inserção de um registro no banco de dados
	 * @param array correlativa entre campos e valores do registro
	 * @return string comando de inserção
	 */
	public function gerarComandoInserir($array) {
		$estrutura = $this->pegarEstrutura();
		$campos = implode(',', array_keys($array));
		$valores = '';
		foreach ($array as $campo => $valor) {
			switch (true) {
				case(empty($valor) && ($campo == $estrutura['chavePrimaria'])):
					$seq = $this->gerarSequencia();
					$valores[] = $seq ? $seq : "null";
					break;
				case(empty($valor)):
					$valores[] = "null";
					break;
				case(is_object($valor)):
					$valor = $this->converterDado($valor);
					$valores[] = (empty($valor)) ? "null" : "'$valor'";
					break;
				default:
					$valores[] = "'" . str_replace("'", "''", $valor) . "'";
			}
		}
		$valores = implode(',', $valores);
		return "insert into {$this->pegarNomeTabela()} ($campos) values ($valores)\n";
	}

	/**
	 * Insere um registro no banco
	 * @param array correlativa entre campos e valores do registro
	 */
	public function inserir($array) {
		$this->executarComando($this->gerarComandoInserir($array));
	}

	/**
	 * Gera o comando de exclusão de um registro no banco de dados
	 * @param string chave primária do registro
	 * @return string o comando de exclusão de um registro no banco de dados
	 */
	public function gerarComandoExcluir($valorChave) {
		$estrutura = $this->pegarEstrutura();
		return "delete from {$this->pegarNomeTabela()} where {$estrutura['chavePrimaria']} = '{$valorChave}'\n";
	}

	/**
	 * Método que verifica se um registro possui dependentes no banco
	 * @return boolean
	 */
	public function possuiDependentes($chave) {
		return false;
	}

	/**
	 * Exclui um registro no banco
	 * @param string chave primária do registro
	 */
	public function excluir($valorChave) {
		$this->executarComando($this->gerarComandoExcluir($valorChave));
	}

	/**
	 * Método que atualiza os registros na arvore
	 * @param string $valor valor de incremento no campo
	 * @param string $chave nome do campo para modificaçao
	 * @param string $posicaoInicial valor inicial do escopo
	 * @param string $posicaoFinal valor final do escopo
	 * @param colecaoPadraoFiltro $filtro agrupamento da arvore
	 */
	public function atualizarArvore($valor, $chave, $posicaoInicial, $posicaoFinal = false, colecaoPadraoFiltro $filtro = null) {
		$estrutura = $this->pegarEstrutura();
		$agrupamento = $this->gerarClausulaDeFiltro($filtro, false);
		if ($agrupamento)
			$agrupamento.=' and  ';
		$filtro = "{$chave} > {$posicaoInicial}";
		if ($posicaoFinal)
			$filtro .= " and {$chave} < {$posicaoFinal}";
		$comando = "update {$this->pegarNomeTabela()} set {$chave}={$chave}{$valor} where {$agrupamento}({$filtro})";
		$this->executarComando($comando);
	}

	/**
	 * Gera o comando de alteração de um registro no banco de dados
	 * @param array correlativa entre campos e valores do registro
	 * @param string chave primária do registro
	 * @return string comando de alteração
	 */
	public function gerarComandoAlterar($array, $valorChave) {
		$estrutura = $this->pegarEstrutura();
		$comando = "update {$this->pegarNomeTabela()} set \n";
		foreach ($array as $campo => $valor) {
			if (empty($valor)) {
				$comando .= "{$campo} = null,\n";
			} else {
				if (is_object($valor)) {
					$valor = $this->converterDado($valor);
					$comando .= empty($valor) ? "{$campo} = null,\n" : "{$campo} = '{$valor}',\n";
				} else {
					$valor = str_replace("'", "''", $valor);
					$comando .= "{$campo} = '{$valor}',\n";
				}
			}
		}
		$comando = substr($comando, 0, -2) . "\n";
		if ($valorChave instanceof colecaoPadraoFiltro) {
			$comando .= $this->gerarClausulaDeFiltro($valorChave, true);
		} else {
			$comando .= "where {$estrutura['chavePrimaria']} = '{$valorChave}'";
		}
		return $comando;
	}

	/**
	 * Altera um registro no banco
	 * @param array array de campos e valores
	 * @param string chave primária do registro
	 */
	public function alterar($array, $valorChave) {
		$this->executarComando($this->gerarComandoAlterar($array, $valorChave));
	}

	//**************************************************************************
	//**************************************************************************
	// 							COMANDOS DDL
	//**************************************************************************
	//**************************************************************************
	/**
	 * Monta o comando de criação da sequence no banco de dados
	 * @return string comando de criação
	 */
	public function gerarComandoCriacaoSequence() {
		return "create sequence {$this->pegarNomeSequencia()}";
	}

	/**
	 * Cria a sequence no banco de dados
	 */
	public function criarSequence() {
		try {
			if (($comandoCriacaoSequence = $this->gerarComandoCriacaoSequence())) {
				$this->executarComando($comandoCriacaoSequence);
			}
		} catch (\Exception $e) {
			$this->executarComando("alter sequence {$this->pegarNomeSequencia()} restart 1;");
		}
	}

	/**
	 * Gera o comando de criacao no banco de dados
	 * @return string comando de criação
	 */
	public function gerarComandoCriacaoTabela() {
		$estrutura = $this->pegarEstrutura();
		$mapeamento = $this->mapeamento();
		$comando = "create table {$this->pegarNomeTabela()} (\n";
		foreach ($estrutura['campo'] as $nomeCampo => $campo) {
			$comando .= $this->gerarComandoCriacaoCampo($mapeamento, $nomeCampo, $campo);
		}
		$comando = substr($comando, 0, -2) . "\n)";
		return $comando;
	}

	/**
	 * Gera o comando de criação do campo da tabela no banco de dados
	 * @param array $mapeamento
	 * @param string $nomeCampo
	 * @param array $campo
	 * @return string
	 */
	public function gerarComandoCriacaoCampo($mapeamento, $nomeCampo, $campo) {
		$campo['tamanho'] = $campo['tamanho'] ? "({$campo['tamanho']})" : '';
		return "	$nomeCampo {$mapeamento[$campo['tipo']]}{$campo['tamanho']} {$mapeamento['obrigatorio'][$campo['obrigatorio']]},\n";
	}

	/**
	 * Cria a tabela no banco de dados
	 */
	public function criarTabela() {
		if (($comandoCriacaoTabela = $this->gerarComandoCriacaoTabela())) {
			$this->executarComando($comandoCriacaoTabela);
		}
	}

	/**
	 * Gera o comando de criacao dos comentários da tabela
	 * @return string comando de criação dos comentários da tabela
	 */
	public function gerarComandoComentarioTabela() {
		$estrutura = $this->pegarEstrutura();
		$inter = $this->internacionalizacao();
		return sprintf("COMMENT ON TABLE %s \n\tIS '%s'", $this->pegarNomeTabela(), $inter->pegarNome());
	}

	/**
	 * Gera os comandos de criacao dos comentários dos campos da tabela
	 * @return array comandos de criação dos comentários dos campos da tabela
	 */
	public function gerarComandoComentarioCampos() {
		$estrutura = $this->pegarEstrutura();
		$inter = $this->internacionalizacao();
		$comandos = array();
		foreach ($estrutura['campo'] as $nomeCampo => $campo) {
			$comandos[$nomeCampo] = sprintf("COMMENT ON COLUMN %s.%s \n\tIS '%s'", $this->pegarNomeTabela(), $nomeCampo, $inter->pegarPropriedade($campo['propriedade'], 'descricao'));
		}
		return $comandos;
	}

	/**
	 * Cria os comentários da tabela no banco de dados
	 */
	public function criarComentarioTabela() {
		if (($inter = $this->internacionalizacao())) {
			$this->executarComando($this->gerarComandoComentarioTabela());
		}
	}

	/**
	 * Cria os comentários dos campos da tabela no banco de dados
	 */
	public function criarComentarioCampos() {
		if (($inter = $this->internacionalizacao())) {
			foreach ($this->gerarComandoComentarioCampos() as $comando) {
				$this->executarComando($comando);
			}
		}
	}

	/**
	 * Monta o comando de criação da chave primaria da tabela
	 * @return string comando de criação
	 */
	public function gerarComandoCriacaoChavePrimaria() {
		$estrutura = $this->pegarEstrutura();
		$comando = "";
		if ($estrutura['chavePrimaria']) {
			$comando .= "alter table only {$this->pegarNomeTabela()} \n\tadd constraint {$this->pegarNomeTabela(false)}_pk primary key ({$estrutura['chavePrimaria']})";
		}
		return $comando;
	}

	/**
	 * Cria a chave primária da tabela no banco de dados
	 */
	public function criarChavePrimaria() {
		if (($comandoCriacaoChavePrimaria = $this->gerarComandoCriacaoChavePrimaria())) {
			$this->executarComando($comandoCriacaoChavePrimaria);
		}
	}

	/**
	 * Monta o comando de criação das chaves estrangeiras no banco de dados
	 * @return string comando de criação
	 */
	public function gerarComandoCriacaoChavesEstrangeiras() {
		$estrutura = $this->pegarEstrutura();
		$comando = "";
		foreach ($estrutura['campo'] as $nomeCampo => $referencia) {
			if (isset($referencia['chaveEstrangeira'])) {
				$arTabelaExtrangeira = explode('.', $referencia['chaveEstrangeira']['tabela']);
				$tabelaExtrangeira = $referencia['chaveEstrangeira']['tabela'];
				if (count($arTabelaExtrangeira) == 1) {
					$tabelaExtrangeira = $this->pegarNomeSchema() . $tabelaExtrangeira;
				}
				$comando .= "alter table only {$this->pegarNomeTabela()} \n\tadd constraint {$this->pegarNomeTabela(false)}_{$nomeCampo}_fk foreign key ($nomeCampo) \n\treferences {$tabelaExtrangeira}({$referencia['chaveEstrangeira']['campo']});";
			}
		}
		return $comando;
	}

	/**
	 * Cria um array contendo as relações da tabela com suas chaves extrangeiras
	 * @return array Chaves extrangeiras
	 */
	public function gerarRelacoesDeChavesEstrangeiras() {
		$chaves = array();
		$estrutura = $this->pegarEstrutura();
		foreach ($estrutura['campo'] as $nomeCampo => $referencia) {
			if (isset($referencia['chaveEstrangeira'])) {
				$arTabelaExtrangeira = explode('.', $referencia['chaveEstrangeira']['tabela']);
				$tabelaExtrangeira = $referencia['chaveEstrangeira']['tabela'];
				if (count($arTabelaExtrangeira) == 1) {
					$tabelaExtrangeira = $this->pegarNomeSchema($this->pegarNomeSchema()).$tabelaExtrangeira;
				}
				$chaves[strtolower($tabelaExtrangeira)] = " {$this->pegarNomeTabela()}.{$nomeCampo} = {$tabelaExtrangeira}.{$referencia['chaveEstrangeira']['campo']} ";
			}
		}
		return $chaves;
	}

	/**
	 * Cria as chaves estrangeiras da tabela no banco de dados
	 */
	public function criarChavesEstrangeiras() {
		if (($comandoCriacaoChavesEstrangeiras = $this->gerarComandoCriacaoChavesEstrangeiras())) {
			$arComandos = explode(';', $comandoCriacaoChavesEstrangeiras);
			foreach ($arComandos as $comando) {
				if ($comando)
					$this->executarComando($comando);
			}
		}
	}

	/**
	 * @return string comando de criação de restrição de domínios no banco de dados
	 */
	public function gerarComandoRestricao() {
		$estrutura = $this->pegarEstrutura();
		$comando = "";
		// Criação de CHECK CONSTRAINT
		foreach ($estrutura['campo'] as $nomeCampo => $campo) {
			if (isset($campo['valores'])) {
				$valores = null;
				foreach ($campo['valores'] as $valor) {
					$valores .="'$valor',";
				}
				$valores = substr($valores, 0, -1);
				$comando .= "alter table only {$this->pegarNomeTabela()} \n\tadd check ({$nomeCampo} in ({$valores}))";
			}
		}
		return $comando;
	}

	/**
	 * Cria as restrições de domínio da tabela no banco de dados
	 */
	public function criarRestricoes() {
		if (($comandoRestricao = $this->gerarComandoRestricao())) {
			$this->executarComando($comandoRestricao);
		}
	}

	/**
	 * Executa o comando de criacao no banco de dados
	 */
	public function criar() {
		$this->criarSequence();
		$this->criarTabela();
		$this->criarComentarioTabela();
		$this->criarComentarioCampos();
		$this->criarChavePrimaria();
		$this->criarChavesEstrangeiras();
		$this->criarRestricoes();
	}

	/**
	 * Gera o comando de destruição no banco de dados
	 * @return string comando de destruição
	 */
	public function gerarComandoDestruicaoSequence() {
		$estrutura = $this->pegarEstrutura();
		return "drop sequence if exists {$this->pegarNomeSequencia()}";
	}

	/**
	 * Executa o comando de destruição da sequence no banco de dados
	 * @return boolean retorno de destruição da sequence
	 */
	public function destruirSequence() {
		try {
			if (($comandoDestruicaoSequence = $this->gerarComandoDestruicaoSequence())) {
				$this->executarComando($comandoDestruicaoSequence);
			}
			return true;
		} catch (\Exception $e) {
			return true;
		}
	}

	/**
	 * Gera o comando de destruição no banco de dados
	 * @return string comando de destruição
	 */
	public function gerarComandoDestruicaoTabela() {
		$estrutura = $this->pegarEstrutura();
		return $comando = "drop table if exists {$this->pegarNomeTabela()} cascade";
	}

	/**
	 * Executa o comando de destruição da tabela no banco de dados
	 * @return boolean retorno de destruição da tabela
	 */
	public function destruirTabela() {
		try {
			$this->ler(null);
			if (($comandoDestruicaoTabela = $this->gerarComandoDestruicaoTabela())) {
				$this->executarComando($comandoDestruicaoTabela);
			}
			return true;
		} catch (\Exception $e) {
			return true;
		}
	}

	/**
	 * Executa o comando de destruição no banco de dados
	 */
	public function destruir() {
		$this->destruirSequence();
		$this->destruirTabela();
		return true;
	}

	/**
	 * Executa o comando de criacao no banco de dados
	 */
	public function recriar() {
		if ($this->existeTabela())
			$this->destruir();
		$this->criar();
	}

	/**
	 * Retorna o comando de criação da entidade no banco de dados ;
	 */
	public function comandoDestruicaoCompleto() {
		$comando = '';
		if (($comandoDestruicaoSequence = $this->gerarComandoDestruicaoSequence())) {
			//$comando = "-- Comando de destruição da sequence\n";
			$comando.= "{$comandoDestruicaoSequence};\n";
		}
		if (($comandoDestruicaoTabela = $this->gerarComandoDestruicaoTabela())) {
			//$comando.= "-- Comando de destruição da tabela\n";
			$comando.= "{$comandoDestruicaoTabela};\n";
		}
		return $comando;
	}

	/**
	 * Retorna o comando de criação da entidade no banco de dados ;
	 */
	public function comandoCriacaoCompleto() {
		$comando = '';
		if (($comandoCriacaoSequence = $this->gerarComandoCriacaoSequence())) {
			//$comando = "\n--[  Comando de criação da sequence  ]\n";
			$comando.= "{$comandoCriacaoSequence};\n";
		}
		if (($comandoCriacaoTabela = $this->gerarComandoCriacaoTabela())) {
			//$comando.= "\n--[  Comando de criação da tabela  ]\n";
			$comando.= "{$comandoCriacaoTabela};\n";
		}
		if (($comandoCriacaoChavePrimaria = $this->gerarComandoCriacaoChavePrimaria())) {
			//$comando.= "\n--[  Comando de criação da chave primária  ]\n";
			$comando.= "{$comandoCriacaoChavePrimaria};\n";
		}
		if (($comandoCriacaoChavesEstrangeiras = $this->gerarComandoCriacaoChavesEstrangeiras())) {
			//$comando.= "\n--[  Comando de criação das chaves estrangeiras  ]\n";
			$comando.= "{$comandoCriacaoChavesEstrangeiras};\n";
		}
		if (($comandoRestricao = $this->gerarComandoRestricao())) {
			//$comando.= "\n--[  Comando de criação das restrições  ]\n";
			$comando.= "{$comandoRestricao};\n";
		}
		if (($comandoComentarioTabela = $this->gerarComandoComentarioTabela())) {
			$comando.= "\n--[  Comentario de tabela  ]\n";
			$comando.= "{$comandoComentarioTabela};\n";
		}
		if (($comandoComentarioCampos = $this->gerarComandoComentarioCampos())) {
			//$comando.= "\n--[  Comentario de campos  ]\n";
			$comandoComentarioCampos = implode(";\n", $comandoComentarioCampos);
			$comando.= "{$comandoComentarioCampos};\n";
		}
		return $comando;
	}

}

