<?php
/**
 * Classe de mapeamento da entidade spoemendas.acessoperfilabas.
 *
 * @version $Id$
 * @since 2017.06.23
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Spoemendas_Model_Acessoperfilabas: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Jeferson Carlos AraÚjo <jeferson.araujo@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Acessoperfilabas($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Acessoperfilabas($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $etoid 
 * @property int $pflcod 
 * @property int $apaid  - default: nextval('spoemendas.acessoperfilabas_apaid_seq'::regclass)
 */
class Spoemendas_Model_Acessoperfilabas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.acessoperfilabas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'apaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
        'etoid' => array('tabela' => 'emenda.emendatipoorigem', 'pk' => 'etoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'apaativo'=> null,
        'etoid' => null,
        'pflcod' => null,
        'apaid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'apaativo'=> array('Digits'),
            'etoid' => array('Digits'),
            'pflcod' => array('Digits'),
            'apaid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Funcao retorna os perfis de acordo com o sistema do spo - emendas
     * @return array|mixed|NULL
     */
    public function getPerfisModel(){
        $sisId = IDENTIFICADOR_SISTEMA_SPO_EMENDAS;
        $query = <<<DML
                SELECT
                    pflcod AS codigo,
                    pfldsc AS descricao
                FROM
                    seguranca.perfil
                WHERE
                    sisid = {$sisId} AND pflstatus = 'A'
DML;
        return $this->carregar($query);

    }

    /**
     * Funcao retorna as formas de execucao das emendas
     * @return array|mixed|NULL
     */
    public function getFormaExecucaoModel(){
        $query = <<<DML
		SELECT
			etoid AS codigo,
			etodescricao AS descricao
		FROM
			emenda.emendatipoorigem
DML;
        return $this->carregar($query);

    }

    /**
     * @param $apaid
     * @return array|mixed|NULL
     */
    public function getPerfilAbasAcessoModel($apaid)
    {
        if($apaid) {
            $sql = <<<DML
                SELECT
                    *
                FROM
                    {$this->stNomeTabela}
                LEFT JOIN spoemendas.abaacesso
                        USING(apaid)
                WHERE
                    apaid = {$apaid} 

DML;
            return $this->carregar($sql);
        }
    }

    /**
     * Retornar uma Query string para listagem das abas cadastradas
     * @return string
     */
    public function getPerfilAbasListModel($filtro){
        return $sql = <<<DML
                    SELECT
                        apaid,
                        pfldsc,
                        etodescricao,
                        (
                            SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT
                                             CASE
                                                WHEN aacaba = 'iniciativa' THEN 'Iniciativas'
                                                WHEN aacaba = 'beneficiarios' THEN 'Entidades Beneficiadas'
                                                WHEN aacaba = 'responsaveis' THEN 'Passo 1: Responsáveis pela elaboração do Programa de Trabalho'
                                                WHEN aacaba = 'iniciativa-beneficiario' THEN 'Passo 2: Indicação de Iniciativas'
                                                WHEN aacaba = 'subacoes' THEN 'Passo 3: Subações do PAR'
                                                WHEN aacaba = 'impedimentos' THEN 'Programa de Trabalho / Impedimento'
                                                WHEN aacaba = 'anexos-oficio' THEN 'Anexo de Ofícios'
                                            END AS descaba
                                        FROM
                                            spoemendas.abaacesso aac
                                        WHERE
                                            aac.apaid = apa.apaid
                                    ),
                                    '<br>'
                                )
                        ) AS descaba
                    FROM
                       {$this->stNomeTabela} apa
                    INNER JOIN emenda.emendatipoorigem
                            USING(etoid)
                    INNER JOIN seguranca.perfil
                            USING(pflcod) 
                            WHERE 1=1 $filtro AND apaativo = TRUE
                            ORDER BY pfldsc
 
DML;

    }

    /**
     * @param $perfis
     * @param $etoid
     * @return array|mixed|NULL
     */
    public function getPermissaoAbasModel($perfis){
        $sql = <<<DML
        SELECT
        etoid,
		array_agg(aacaba) as perfis
		FROM
		{$this->stNomeTabela} ap
		LEFT JOIN spoemendas.abaacesso aac USING(apaid)
		WHERE pflcod IN ({$perfis}) AND apaativo = TRUE
        GROUP BY etoid
DML;
        return $this->carregar($sql);
    }

    /**
     * @param $perfis
     * @return array|mixed|NULL
     */
    public function getPermissaoFormaExecucaoModel($perfis){
        $sql = <<<DML
        		SELECT
                    array_agg(etoid) AS formaExec
                FROM
                    {$this->stNomeTabela}
                WHERE
                    pflcod IN($perfis) AND apaativo = TRUE
DML;
        return $this->carregar($sql);
    }

    /**
     * verifica se ja existe no banco de dados
     * @param $dados
     * @return array|mixed|NULL
     */
    public function verificaExisteBancoModel($dados)
    {
        $sql = <<<DML
            SELECT
                apaid
            FROM
                spoemendas.acessoperfilabas
                WHERE pflcod = {$dados['pflcod']} AND etoid = {$dados['etoid']} AND apaativo = TRUE
DML;
        return $this->carregar($sql);
    }

    public function getPerfisFormasExecExistModel($perfis)
    {
        if ($perfis) {
            $sql = <<<DML
            SELECT
                etoid
            FROM
                spoemendas.acessoperfilabas
            WHERE
                pflcod =({$perfis}) AND apaativo = TRUE
DML;
            return $this->carregar($sql);
        }
    }

    /**
     * Verifica se usuario possui acesso a todas as formas de execucao
     * @param $dados
     * @return array|mixed|NULL
     */
    public function verificaPossuiAcessTodosModel($dados)
    {
        $sql = <<<DML
            SELECT
                etoid
            FROM
                spoemendas.acessoperfilabas
                WHERE pflcod = {$dados['pflcod']} AND apaativo = TRUE
DML;
        return $this->carregar($sql);
    }

    /**
     * Busca se o usuario pode editar pela forma de execucao
     * @param $perfis
     * @return array|mixed|NULL
     */
    public function EditEmendaFormaExecModel($perfis)
    {
        $sql = <<<DML
            SELECT
                array_agg(etoid) as etoid
            FROM
                spoemendas.periodopreenchimentoemenda
            WHERE
                prsano = '{$_SESSION['exercicio']}'
                AND ppeativo = TRUE
                AND pflcod IN ({$perfis}) 
DML;
        return $this->carregar($sql);

    }

    /**
     * Invativa o registro de acesso a abas
     * @param $apaid
     */
    public function invativaAcessoAbasModel($apaid){
        $sql = <<<DML
            UPDATE
                spoemendas.acessoperfilabas
            SET
                apaativo = FALSE
            WHERE
                apaid = {$apaid}
DML;
        $this->executar($sql);

    }

}
