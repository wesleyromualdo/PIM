<?php
/**
 * Classe de mapeamento da entidade sac.avisofiltro.
 *
 * @version $Id$
 * @since 2016.03.01
 */

/**
 * Sac_Model_Avisofiltro
 *
 * @package Sac\Model
 * @uses Simec\Db\Modelo
 * @author Orion Teles De Mesquita <orion.mesquita@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sac_Model_Avisofiltro($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sac_Model_Avisofiltro($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $avfid 
 * @property int $aviid 
 * @property int $pflcod 
 * @property int $regcod 
 * @property string $estuf 
 * @property string $muncod 
 */
class Sac_Model_AvisoFiltro extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sac.avisofiltro';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'avfid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'aviid' => array('tabela' => 'sac.aviso', 'pk' => 'aviid'),
        'pflcod' => array('tabela' => 'perfil', 'pk' => 'pflcod'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'avfid' => null,
        'aviid' => null,
        'pflcod' => null,
        'regcod' => null,
        'estuf' => null,
        'muncod' => null,
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
            'avfid' => array('Digits'),
            'aviid' => array('Digits'),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'regcod' => array('allowEmpty' => true,'Digits'),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
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

    public function recuperarFiltros($aviid)
    {
        $aRetorno = array('perfil'=>array(), 'regiao'=>array(),'estado'=>array(), 'municipio'=>array());
        if($aviid){
            $sql = "select distinct
                        case
                                when coalesce(af.regcod, 0) != 0 then r.regcod
                                when coalesce(af.estuf, '0') != '0' then e.regcod
                                when coalesce(af.muncod, '0') != '0' then emun.regcod
                        end as regcod,
                        case
                                when coalesce(af.regcod, 0) != 0 then null
                                when coalesce(af.estuf, '0') != '0' then e.estuf
                                when coalesce(af.muncod, '0') != '0' then m.estuf
                        end as estuf,
                        case
                                when coalesce(af.regcod, 0) != 0 then null
                                when coalesce(af.estuf, '0') != '0' then null
                                when coalesce(af.muncod, '0') != '0' then m.muncod
                        end as muncod,
                        pflcod
                    from sac.aviso a
                        inner join sac.avisofiltro af on af.aviid = a.aviid

                        -- Tipo Regional
                        left join territorios.regiao r on r.regcod::int = af.regcod

                        -- Tipo Estadual
                        left join territorios.estado e on e.estuf = af.estuf

                        -- Tipo Municipal
                        left join territorios.municipio m on m.muncod = af.muncod
                        left join territorios.estado emun on emun.estuf = m.estuf
                    where a.aviid = {$aviid} ";

            $dados = $this->carregar($sql);
            $dados = $dados ? $dados : array();
            foreach ($dados as $dado) {
                if($dado['regcod']){
                    $aRetorno['regiao'][trim($dado['regcod'])] = trim($dado['regcod']);
                }
                if($dado['estuf']){
                    $aRetorno['estado'][$dado['estuf']] = $dado['estuf'];
                }
                if($dado['muncod']){
                    $aRetorno['municipio'][$dado['muncod']] = $dado['muncod'];
                }
                if($dado['pflcod']){
                    $aRetorno['perfil'][$dado['pflcod']] = $dado['pflcod'];
                }
            }
        }
        return $aRetorno;
    }

}
