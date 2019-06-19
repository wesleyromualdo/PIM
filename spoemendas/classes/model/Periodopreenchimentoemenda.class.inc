<?php
/**
 * Classe de mapeamento da entidade spoemendas.periodopreenchimentoemenda.
 *
 * @version $Id$
 * @since 2017.06.28
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Spoemendas_Model_Periodopreenchimentoemenda: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Jeferson Carlos AraÚjo <jeferson.araujo@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Periodopreenchimentoemenda($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Periodopreenchimentoemenda($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property Char $prsano
 * @property string $ppemensagem 
 * @property \Datetime(Y-m-d H:i:s) $ppedatafim 
 * @property \Datetime(Y-m-d H:i:s) $ppedatainicio 
 * @property int $pperesultprimario 
 * @property int $etoid 
 * @property int $pflcod 
 * @property int $ppeid  - default: nextval('spoemendas.periodopreenchimentoemenda_ppeid_seq'::regclass)
 */
class Spoemendas_Model_Periodopreenchimentoemenda extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.periodopreenchimentoemenda';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ppeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'etoid' => array('tabela' => 'emenda.emendatipoorigem', 'pk' => 'etoid'),
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ppeativo'=>null,
        'prsano'=>null,
        'ppemensagem' => null,
        'ppedatafim' => null,
        'ppedatainicio' => null,
        'pperesultprimario' => null,
        'etoid' => null,
        'pflcod' => null,
        'ppeid' => null,
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
            'ppeativo'=>array('allowEmpty' => true),
            'prsano'=> array('allowEmpty' => true),
            'ppemensagem' => array('allowEmpty' => false),
            'ppedatafim' => array('allowEmpty' => true),
            'ppedatainicio' => array('allowEmpty' => true),
            'pperesultprimario' => array('Digits'),
            'etoid' => array('Digits'),
            'pflcod' => array('Digits'),
            'ppeid' => array('Digits'),
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
     * Busca os periodos cadastrados pelo usuario
     * @param $ppe
     * @return array|mixed|NULL
     */
    public function getPeriodoPreenchimentoModel($ppeid){
        $sql = <<<DML
                SELECT
                    ppeid,
                    pflcod,
                    etoid,
                    pperesultprimario,
                    to_char(ppedatainicio,'dd/mm/YYYY') AS ppedatainicio,
                    to_char(ppedatafim,'dd/mm/YYYY') AS ppedatafim,
                    ppemensagem,
                    prsano
                    
                FROM
                    {$this->stNomeTabela}
                    WHERE ppeid = {$ppeid} AND ppeativo = TRUE AND prsano = '{$_SESSION['exercicio']}'
                ORDER BY
                    ppeid DESC
DML;
        return $this->carregar($sql)[0];
    }

    /**
     * Busca todos os periodos de preenchimento cadastrados
     * @return array|mixed|NULL
     */
    public function getPeriodosPreenchimentoModel($where){
        $sql = <<<DML
                    SELECT
                        ppeid,
                        pfldsc,
                        etodescricao,
                        pperesultprimario,
                        to_char(ppedatainicio,'dd/mm/YYYY') ||' a '|| to_char(ppedatafim,'dd/mm/YYYY') AS periodo,
                        ppemensagem
                        
                        
                    FROM
                        {$this->stNomeTabela}
                        INNER JOIN seguranca.perfil USING(pflcod)
                        INNER JOIN emenda.emendatipoorigem USING(etoid)
                        WHERE 1=1 
                        AND ppeativo = TRUE {$where} 
                        AND ppeativo = TRUE 
                        AND prsano = '{$_SESSION['exercicio']}'
                    ORDER BY
                        ppeid DESC
DML;
        return $sql;

    }

    /**
     * Busca as permissoes do periodo para preenchimento
     * @param $pflcod
     * @return array|mixed|NULL
     */
    public function getPermissaoPreenchimentoModel($pflcod)
    {
        $sql = <<<DML
	         SELECT
                        prsano,
                        ppeid,
                        pflcod,
                        etoid,
                        pperesultprimario,
                        to_char(ppedatainicio,'dd/mm/YYYY') AS ppedatainicio,
                        to_char(ppedatafim,'dd/mm/YYYY') AS ppedatafim,
                        ppemensagem
            FROM
                {$this->stNomeTabela}
            WHERE pflcod IN ({$pflcod}) 
            AND to_char(NOW(),'dd/mm/YYYY')::date >= to_char(ppedatainicio,'dd/mm/YYYY')::date 
            AND to_char(NOW(),'dd/mm/YYYY')::date <= to_char(ppedatafim,'dd/mm/YYYY')::date
            AND ppeativo = TRUE
            AND prsano = '{$_SESSION['exercicio']}'
DML;
        return $this->carregar($sql);

    }

    public function getMensagensPeriodoModel($perfis)
    {
        $sql = <<<DML
                SELECT 
                    TO_CHAR (ppedatainicio, 'DD/MM/YYYY') AS ppedatainicio,
                    TO_CHAR (ppedatafim, 'DD/MM/YYYY') AS ppedatafim,
                    TO_CHAR(ppedatainicio, 'YYYY') as anoperiodo,
                   regexp_replace (regexp_replace (
                                      regexp_replace (ppemensagem, '(\\\\[_DATA_INICIAL_\\\\])+',
                                      TO_CHAR (ppedatainicio, 'DD/MM/YYYY')), '(\\\\[_DATA_FINAL_\\\\])+',
                                      TO_CHAR (ppedatafim, 'DD/MM/YYYY')), '(\\\\[_DIAS_RESTANTES_\\\\])+',
                                  date_part ('day', ppedatafim :: TIMESTAMP - ppedatainicio :: TIMESTAMP) || '') as ppemensagem
              FROM {$this->stNomeTabela} ppe
             WHERE pflcod IN ({$perfis}) 
             AND prsano = '{$_SESSION['exercicio']}'
             AND ppeativo = TRUE
             AND prsano = '{$_SESSION['exercicio']}'
DML;
        return $this->carregar($sql);
    }

    /**
     * Retorna dados caso ja exista cadastrado periodo
     * @return array|mixed|NULL
     */
    public function ExistePeriodoCadastradoModel($dados)
    {
        $sql = <<<DML
                SELECT
                    ppeid
                FROM
                    {$this->stNomeTabela}
                WHERE
                    pflcod = {$dados['pflcod']}
                    AND etoid = {$dados['etoid']}
                    AND prsano = '{$dados['prsano']}'
                    AND pperesultprimario = {$dados['pperesultprimario']}
                    AND ppeativo = TRUE
                    AND prsano = '{$_SESSION['exercicio']}'
DML;
        return $this->carregar($sql);

    }

    /**
     * Inativa o periodo para preenchimento da emenda
     * @return array|mixed|NULL
     */
    public function inativarPeriodoModel($ppeid)
    {
        $sql = <<<DML
        UPDATE {$this->stNomeTabela} SET ppeativo  = FALSE WHERE ppeid = {$ppeid} 
DML;
       return  $this->executar($sql);
    }

}
