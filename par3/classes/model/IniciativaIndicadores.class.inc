<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_indicadores.
 *
 * @version $Id$
 * @since 2017.06.27
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_IniciativaIndicadores: Tabela utilizada para cadastro da Iniciativa Detalhe - Indicadores (3° Aba).
Story #11025
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIndicadores($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIndicadores($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $idiid Chave Primária - default: nextval('par3.iniciativa_indicadores_idiid_seq'::regclass)
 * @property int $iniid FK da Iniciativa (par3.iniciativa)
 * @property int $dimid FK da Dimensão (par3.dimensao)
 * @property int $areid FK da Área (par3.area)
 * @property int $indid FK do Indicador (par3.indicador)
 * @property int $crtid FK do Componentes do Indicador (par3.criterio)
 */
class Par3_Model_IniciativaIndicadores extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_indicadores';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'idiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
        'indid' => array('tabela' => 'par3.indicador', 'pk' => 'indid'),
        'dimid' => array('tabela' => 'par3.dimensao', 'pk' => 'dimid'),
        'crtid' => array('tabela' => 'par3.criterio', 'pk' => 'crtid'),
        'areid' => array('tabela' => 'par3.area', 'pk' => 'areid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'idiid' => null,
        'iniid' => null,
        'dimid' => null,
        'areid' => null,
        'indid' => null,
        'crtid' => null,
        'idistatus' => null,
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
            'idiid' => array('Digits'),
            'iniid' => array('Digits'),
            'dimid' => array('Digits'),
            'areid' => array('Digits'),
            'indid' => array('Digits'),
            'crtid' => array('Digits'),
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

    public function recuperarPorIniciativa($iniid,$arrPost = array())
    {
        $where = array();
        $where[] = "iniid = {$iniid}";
        $where[] = $arrPost['dimid'] ? "dimid = {$arrPost['dimid']}":'';

        return $this->recuperarTodos('*',$where);
    }

    public function recuperarIniciativaAreas($arrPost = array())
    {
        $sql = "
              SELECT DISTINCT are.aredsc,are.areid,idi.iniid,are.arecod
              FROM par3.iniciativa_indicadores idi
              INNER JOIN par3.area are ON idi.areid = are.areid AND are.dimid = idi.dimid
              WHERE idi.iniid     = {$arrPost['iniid']}
              AND   idi.dimid     = {$arrPost['dimid']}
              AND   idi.idistatus = 'A' 
        ";
        return $this->carregar($sql);
    }

    public function recuperarIniciativaAreasIndicadores($arrPost = array())
    {
        $sql = "
              SELECT 
                    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.indajuda,
				    ind.areid,
				    ind.indrequerido
              FROM par3.iniciativa_indicadores idi
              INNER JOIN par3.indicador ind ON idi.indid = ind.indid
              WHERE idi.iniid = {$arrPost['iniid']}
              AND   idi.dimid = {$arrPost['dimid']}
              AND   idi.areid = {$arrPost['areid']}
              AND   idi.idistatus = 'A' 


        ";
        return $this->carregar($sql);
    }
}
