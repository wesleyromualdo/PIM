<?php
/**
 * Implementa a classe de configuração do extrato dinamico.
 *
 * @version $Id: Config.php 109733 2016-04-04 20:24:04Z maykelbraz $
 */

require_once dirname(__FILE__) . '/../ExtratoDinamico.php';

/**
 * Classe de armazenamento e manipulação das configurações de extratos dinâmicos.
 *
 * @author Maykel S. Braz <maykel.braz@mec.gov.br>
 * @package Simec\ExtratoDinamico
 */
class Simec_ExtratoDinamico_Config
{
    /**
     * @var string Nome do módulo ao qual o extrato pertence - use o valor de sisdiretorio
     */
    protected $modulo;
    /**
     * @var string Nome do relatório - default é "extrato"
     */
    protected $nome;
    /**
     * @var string Nome da view que será utilizada para a consulta do extrato - esquema.nomedaview
     */
    protected $dbView;
    /**
     * @var int Id do extrato no banco de dados.
     */
    protected $dbId;
    /**
     * @var mixed[] Armazena as configurações do relatorio.
     */
    protected $config = [
        Simec_ExtratoDinamico::QUALITATIVO => [],
        Simec_ExtratoDinamico::QUANTITATIVO => [],
        Simec_ExtratoDinamico::TOTALIZADO => [],
        Simec_ExtratoDinamico::CALLBACK => [],
        Simec_ExtratoDinamico::FILTRO => [],
    ];

    /**
     * Cria uma nova instância da configuração de extrato, carregando as configurações do relatório indicado.
     *
     * @param string $modulo Nome do módulo ao qual o extrato pertence.
     * @param string $nome Nome do relatório.
     */
    public function __construct($modulo, $nome)
    {
        $this->setModulo($modulo)
            ->setNome($nome)
            ->carregar();
    }

    /**
     * Define o nome do módulo.
     *
     * @param string $modulo Nome do módulo ao qual o extrato pertence.
     * @return \Simec_ExtratoDinamico_Config
     * @throws Exception
     */
    public function setModulo($modulo)
    {
        if (empty($modulo)) {
            throw new Exception('O módulo do relatório não pode ser vazio.');
        }
        $this->modulo = $modulo;
        return $this;
    }

    public function getModulo()
    {
        return $this->modulo;
    }

    public function setNome($nome)
    {
        if (empty($nome)) {
            throw new Exception('O nome do relatório não pode ser vazio.');
        }

        $this->nome = $nome;
        return $this;
    }

    public function getNome()
    {
        return $this->nome;
    }

    public function getDbView()
    {
        return $this->dbView;
    }

    public function getDbId()
    {
        return $this->dbId;
    }

    protected function carregar()
    {
        $modulo = $this->getModulo();
        $relatorio = $this->getNome();

        $configBusca = [
            "edostatus = 'A'",
            "exdmodulo = '{$modulo}'",
            "exdrelatorio = '{$relatorio}'"
        ];

        $dadosConfig = (new Public_Model_Extratodinamicoconfig())
            ->recuperarTodos('*', $configBusca, 'edotipo, edolabel', ['leftjoin' => 'exdid']);

        if (empty($dadosConfig)) {
            throw new Exception("Nenhuma configuração encontrada para o relatório solicitado ({$modulo}/{$relatorio}).");
        }

        foreach ($dadosConfig as $config) {
            $this->config[$config['edotipo']][$config['edocoluna']] = [
                'expressao' => $config['edoexpressao'],
                'label' => $config['edolabel'],
                'callback' => $config['edocallback'],
            ];

            if (!empty($config['edoquery'])) {
                $this->config[Simec_ExtratoDinamico::FILTRO][$config['edocoluna']] = [
                    'label' => $config['edolabel'],
                    'query' => $config['edoquery'],
                ];
            }

            if (!empty($config['edocallback'])) {
                $this->config[Simec_ExtratoDinamico::CALLBACK][$config['edocoluna']] = $config['edocallback'];
            }

            if ('t' == $config['edototalizador']) {
                $this->config[Simec_ExtratoDinamico::TOTALIZADO][] = $config['edocoluna'];
            }
        }

        $this->dbView = $config['exdview'];
        $this->dbId = $config['exdid'];





        return $this;
    }

    public function getColunasQualitativas()
    {
        return $this->getColunas(Simec_ExtratoDinamico::QUALITATIVO);
    }

    public function getColunasQuantitativas()
    {
        return $this->getColunas(Simec_ExtratoDinamico::QUANTITATIVO);
    }

    public function getFiltros()
    {
        return $this->config[Simec_ExtratoDinamico::FILTRO];
    }

    public function getColunasTotalizadas()
    {
        return $this->config[Simec_ExtratoDinamico::TOTALIZADO];
    }

    protected function getColunas($tipo)
    {
        if (empty($this->config)) {
            $this->carregar();
        }

        $dados = [];
        foreach ($this->config[$tipo] as $codigo => $config) {
            $dados[] = [
                'codigo' => $codigo,
                'descricao' => $config['label']
            ];
        }
        return $dados;
    }

    /**
     * Retorna o título de uma coluna ou uma lista de títulos de colunas.
     *
     * @param string|string[] $coluna Nome da coluna ou lista de nomes para retorno do título.
     * @return string|string[]
     */
    public function getTituloColuna($coluna)
    {
        if (!is_array($coluna)) {
            return array_key_exists($coluna, $this->config[Simec_ExtratoDinamico::QUALITATIVO])
                ?$this->config[Simec_ExtratoDinamico::QUALITATIVO][$coluna]['label']
                :$this->config[Simec_ExtratoDinamico::QUANTITATIVO][$coluna]['label'];
        }

        $titulos = [];
        foreach ($coluna as $col) {
            $titulos[] = $this->getTituloColuna($col);
        }

        return $titulos;
    }

    public function getCallbackColuna($coluna)
    {
        if (array_key_exists($coluna, $this->config[Simec_ExtratoDinamico::CALLBACK])) {
            return $this->config[Simec_ExtratoDinamico::CALLBACK][$coluna];
        }

        return null;
    }

    public function getSelectColuna($coluna)
    {
        if (array_key_exists($coluna, $this->config[Simec_ExtratoDinamico::QUALITATIVO])) {
            $dadosColuna = $this->config[Simec_ExtratoDinamico::QUALITATIVO][$coluna];
        } else {
            $dadosColuna = $this->config[Simec_ExtratoDinamico::QUANTITATIVO][$coluna];
        }

        if (empty($dadosColuna['expressao'])) {
            return $coluna;
        }

        return "{$dadosColuna['expressao']} AS {$coluna}";
    }
}
