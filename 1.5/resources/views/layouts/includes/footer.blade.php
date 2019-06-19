<div class="footer">

    <div class="footer">
        <div class="pull-right">
            <strong>Data:</strong>
            {{date("d/m/Y - H:i:s")}}|
            <strong>Último acesso</strong>
            {{ date('d/m/Y - H:i:s', strtotime($user->usuacesso)) }}
        </div>

    </div>
</div>

@include('layouts.includes.topnavbar_userinfo')

@include('layouts.includes.footer_alert_modal')
