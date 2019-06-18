<?php
/**
 * Interface de definição de Renderers de Simec_Listagem.
 * $Id: Abstract.php 94624 2015-03-02 17:12:22Z maykelbraz $
 */

abstract class Simec_Listagem_Renderer_Abstract
{

    /**
     * Armazena os dados utilizados para montar a listagem.
     * @var array
     * @see Simec_Listagem_Renderer_Abstract::setDados()
     */
    protected $dados;

    /**
     * Armazena o título do relatório.
     * Se estiver vazio, nenhum título é exibido.
     * @var string
     * @see Simec_Listagem::setTitulo()
     * @See Simec_Listagem::renderTitulo()
     */
    protected $titulo = '';

    /**
     * Lista de colunas utilizadas como cabeçalho da listagem.
     *
     * @var array
     * @see Simec_Listagem::setCabecalho()
     */
    protected $cabecalho;

    /**
     * Lista de colunas que não aparecem na listagem.
     * @var array
     * @see Simec_Listagem::esconderColunas()
     */
    protected $colunasOcultas = array();

    /**
     * Lista de colunas totalizadas com a soma dos valores processados até o momento.
     * $this->colunasTotalizadas = array(
     *      'nomedocampo' => valor
     * );
     *
     * @var array
     * @see Simec_Listagem::setTotalizador()
     * @see Simec_Listagem::totalizarColunas()
     */
    protected $colunasTotalizadas = array();

    /**
     * Atributo de ajuda para renderização do titulo.
     *
     * @var bool
     * @todo Remover a utilização deste campo.
     * @see Simec_Listagem::setDados()
     * @see Simec_Listagem::renderCabecalho()
     */
    protected $renderPrimeiroItem = true;

    /**
     * Cada elemento é uma callback para um campo. Toda vez que o campo for ser impresso na tabela, ele primeiro é
     * processado pela callback registrada e o resultado do processamento é impresso no lugar do valor do campo.
     * $callbacksDeCampo = array(
     *      'nome_do_campo' => nome_da_callback
     * );
     * @var array
     * @see Simec_Listagem::addCallbackDeCampo()
     */
    protected $callbacksDeCampo = array();

    /**
     * Cada elemento define uma regra de formatação de linha. As regras são compostas pelos seguintes elementos:
     * $regrasDeLinha = array(
     *      array(
     *            'campo' => nome_do_campo,
     *            'op' => igual|diferente|contem,
     *            'valor' => valor_para_comparacao_com_campo
     *            'classe' => nome_da_classe_css_de_modificacao
     *      ),
     * );
     * @var array
     * @see Simec_Listagem::addRegraDeLinha()
     */
    protected $regrasDeLinha = array();



    public function __construct(array $dados = null)
    {
        if (!empty($dados)) {
            $this->setDados($dados);
        }
    }

    /**
     * Carrega no objeto os dados que serão utilizados para criar a listagem.
     *
     * @param array $dados Dados para criação da listagem.
     */
    public function setDados($dados)
    {
        if (!is_array($dados)) {
            return false;
        }

        $this->dados = $dados;

        // -- Limpando somadores anteriores
        foreach ($this->colunasTotalizadas as &$valor) {
            $valor = 0;
        }
        // -- Limpando indicador do primeiro campo a ser renderizado
        $this->renderPrimeiroItem = true;

        return $this;
    }

    public function semDados()
    {
        return empty($this->dados);
    }

    /**
     * Define um título para o relatório.
     * @param string $titulo Título a ser exibido acima do relatório.
     */
    public function setTitulo($titulo)
    {
        if (!empty($titulo)) {
            $this->titulo = $titulo;
        }
    }

    /**
     * Lista de títulos das colunas do relatório. Também cria títulos de duas colunas,
     * para isso, passe o nome da coluna principal como chave do array e as colunas filhas como
     * itens deste array.
     * Exemplo cabecalho simples:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * Exemplo cabecalho de dois níveis:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));
     * Obs: Se passar a string 'auto', serão utilizados os nomes das colunas presentes
     * no conjunto de dados da listagem.
     *
     * @param array|string $cabecalho Array com o título de cada coluna, ou a palavra 'auto'.
     * @todo Transformar o valor 'auto' no padrão da classe.
     */
    public function setCabecalho($cabecalho)
    {
        if (('auto' != $cabecalho) && !is_array($cabecalho)) {
            throw new Exception("\$cabecalho deve ser um array, ou o valor 'auto'.");
        }
        $this->cabecalho = $cabecalho;
    }

    /**
     * Indica que um, ou mais, campo(s) da query não será(ão) exibido(s).
     *
     * @param string|array $nomeColuna Nome do campo da query que não será exibida na listagem.
     * @return \Simec_Listagem
     */
    public function esconderColunas($nomeColuna)
    {
        if (!is_array($nomeColuna)) {
            $this->colunasOcultas[] = $nomeColuna;
        } else {
            // -- Recebendo um array de campo para esconder as acolunas
            foreach ($nomeColuna as $coluna) {
                $this->colunasOcultas[] = $coluna;
            }
        }

        return $this;
    }

    protected function somarColuna($nomeCampo, $valor)
    {
        if (strpos($valor, '.')) {
            $valor = (double)$valor;
        } else {
            $valor = (int)$valor;
        }
        $this->colunasTotalizadas[$nomeCampo] += $valor;
    }

    /**
     * Adiciona uma função callback de processamento de conteúdo de campo.
     * Uma ação comum que pode ser executada com este método, é a aplicação de máscara em um campo de CPF.
     *
     * Exemplo de utilização:<pre>
     * function mascaraReal($valor)
     * {
     * &nbsp;&nbsp;&nbsp;&nbsp;return "R$ {$valor}";
     * }
     * [...]
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();</pre>
     *
     * @param string|array $nomeCampo Nome(s) do(s) campo(s) que receberá(ão) o tratamento.
     * @param string $nomeCallback Nome da função de processamento do campo. Ela deve retornar sempre uma string.
     * @throws Exception Gerada quando o nome da callback ou a própria função é inválida.
     */
    public function addCallbackDeCampo($nomeCampo, $nomeCallback)
    {
        if (empty($nomeCampo)) {
            return;
        }

        if (empty($nomeCallback)) { // -- Foi informado o nome da função?
            throw new Exception('O nome da função de callback do campo nao pode ser vazia.');
        }
        if (!is_callable($nomeCallback)) { // -- A função foi declarada??
            throw new Exception("A função '{$nomeCallback}' não está declarada.");
        }

        // -- Recebendo um array de campo para adicionar como callback
        if (!is_array($nomeCampo)) {
            $this->callbacksDeCampo[$nomeCampo] = $nomeCallback;
        } else {
            foreach ($nomeCampo as $campo) {
                $this->callbacksDeCampo[$campo] = $nomeCallback;
            }
        }
    }

    /**
     * Configura o tipo de totalizador da listagem. Adicionalmente, pode
     * definir quais colunas serão totalizadas.
     *
     * @param int $totalizador Define o tipo de totalizador
     * @param string|array $colunas Coluna (ou lista de colunas) que serão totalizadas.
     * @return \Simec_Listagem
     * @see Simec_Listagem::totalizarColunas();
     * @see Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @see Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        if ($totalizador != Simec_Listagem::TOTAL_QTD_REGISTROS && $totalizador != Simec_Listagem::TOTAL_SOMATORIO_COLUNA) {
            throw new Exception(
                'Tipo de totalizador inválido. Tipos válidos: Simec_Listagem:TOTAL_QTD_REGISTROS ou Simec_Listagem::TOTAL_SOMATORIO_COLUNA.'
            );
        }

        if (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $totalizador && !is_null($colunas)) {
            $this->totalizarColunas($colunas);
        }

        $this->totalizador = $totalizador;
        return $this;
    }

    /**
     * Define quais colunas serão totalizadas.
     *
     * @param string|array $campos
     */
    public function totalizarColunas($campos)
    {
        if (empty($campos)) {
            return $this;
        }

        if (!is_array($campos)) {
            $this->colunasTotalizadas[$campos] = 0;
        } else {
            foreach ($campos as $campo) {
                $this->colunasTotalizadas[$campo] = 0;
            }
        }
    }

    /**
     * Adiciona uma nova regra de formatação de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     * @todo validar a estrutura da nova regra a ser adicionada
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->regrasDeLinha[] = $regra;
    }

    /**
     * Executa a criação da listagem de acordo com o Delegate implementado.
     */
    abstract public function render();

    abstract protected function renderTitulo();

    abstract protected function renderCabecalho();

    abstract protected function renderRodape();
}